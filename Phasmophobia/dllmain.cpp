/*
PhasmaHook is a fork phasmophobia mod-cheat, with more or less convenient GUI, with support for basic functions, 
such as infinite sprint, skeletal wallhack on the ghost, esp line with distance display and corresponding color palette,
and all information about the ghost. 
In the future, this project can be improved, but more data about the structures is needed.

by 0xcds4r
23.09.2024
*/

#include "Init.h"
#include "library/Console.hpp"
#include "library/D3D11Hook.h"
#include "library/HotKey.hpp"
#include "library/imgui/Font.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"
#include "library/imgui/imgui_internal.h"
#include <TlHelp32.h>
#include "features/Players/Player.hpp"
#include "GUI.h"
#include "library/imgui/imgui_notify.h"
#include "dllmain.h"

Gui gui;

char ProcessInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return gui.ProcessInput(hWnd, msg, wParam, lParam);
}

bool InitializeImGui() {
	if (!init_space::Info::imGuiInit) {
		LOG_FMT_DEBUG("Phasmohook initializing gui..");

		UnityResolve::ThreadAttach();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();

		auto& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard;
		//io.Fonts->ConfigData.Data->FontDataOwnedByAtlas = false;
		io.Fonts->AddFontFromMemoryTTF(Font::getallfont(), 2362740, 15, nullptr, io.Fonts->GetGlyphRangesCyrillic());

		ImGui_ImplWin32_Init(dx_hook::Hk11::GetHwnd());
		ImGui_ImplDX11_Init(dx_hook::Hk11::GetDevice(), dx_hook::Hk11::GetContext());

		dx_hook::Hk11::SetWndProc([](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> char {
			return ProcessInput(hWnd, msg, wParam, lParam);
			});

		gui.ApplyStyles();

		init_space::Info::imGuiInit = true;
		init_space::Info::mainShow = false;
		init_space::Info::tipsShow = false;
	}
	return init_space::Info::imGuiInit;
}

void RenderImGui() {
	if (!InitializeImGui()) {
		return;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (init_space::Info::mainShow) {
		gui.RenderMainWindow();
	}

	gui.DoDrawFeatures();

	ImGui::EndFrame();
	ImGui::Render();
	dx_hook::Hk11::GetContext()->OMSetRenderTargets(1, dx_hook::Hk11::GetTargetView(), nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void FeatureUpdateLoop() 
{
	gui.updateLoop(std::chrono::high_resolution_clock::now());

	tagRECT rect;
	while (true) {
		GetClientRect(dx_hook::Hk11::GetHwnd(), &rect);
		init_space::Info::w = rect.right - rect.left;
		init_space::Info::h = rect.bottom - rect.top;
		drawMath::UpdateResolutionScale();
		Sleep(100);

		for (const auto& feature : init_space::Feature::features | std::views::values) {
			for (const auto& func : feature) {
				if (func->GetInfo().needUpdate) {
					func->Update();
				}
			}
		}
	}
}

auto APIENTRY DllMain(HMODULE hModule, const DWORD ul_reason_for_call, LPVOID lpReserved) -> BOOL {
	SetConsoleOutputCP(CP_UTF8);

	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		init_space::Info::hModule = hModule;

		if (HMODULE phasm = GetModuleHandle(L"Phasmophobia.exe")) {
			std::thread([hModule] {
				console::StartConsole(L"Console", false);
				LOG_FMT_DEBUG("Phasmohook injecting..");

				UnityResolve::Init(GetModuleHandleA("GameAssembly.dll"), UnityResolve::Mode::Il2Cpp);
				init_space::Feature::Init();

				dx_hook::Hk11::Build([] { RenderImGui(); });
				std::thread(FeatureUpdateLoop).detach();
				}).detach();
		}
	}
	return TRUE;
}

auto CALLBACK HookCallBack(int Code, WPARAM wParam, LPARAM lParam) -> LRESULT {
    std::wstring file(255, '\0');
    GetModuleFileNameW(init_space::Info::hModule, file.data(), 255);
    LoadLibraryW(file.c_str());
    return CallNextHookEx(nullptr, Code, wParam, lParam);
}

auto GetIdByName(const std::wstring& name) -> DWORD {
	HANDLE hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hsnapshot == INVALID_HANDLE_VALUE) return -1;

	PROCESSENTRY32 processer;
	processer.dwSize = sizeof(PROCESSENTRY32);

	for (int flag = Process32First(hsnapshot, &processer); flag; flag = Process32Next(hsnapshot, &processer)) {
		if (processer.szExeFile == name) {
			CloseHandle(hsnapshot);
			return processer.th32ProcessID;
		}
	}

	CloseHandle(hsnapshot);
	return -2;
}

static DWORD idThread;

auto CALLBACK EnumFunc(HWND hWnd, LPARAM pid) -> BOOL {
	DWORD dwProcessId;
	if (GetWindowThreadProcessId(hWnd, &dwProcessId) && dwProcessId == pid && IsWindowVisible(hWnd)) {
		idThread = GetWindowThreadProcessId(hWnd, nullptr);
		return FALSE; // Stop enumeration
	}
	return TRUE; // Continue enumeration
}

extern "C" _declspec(dllexport) auto Inject() -> void {
	if (auto pid = GetIdByName(L"Phasmophobia.exe"); pid != -1) {
		EnumWindows(EnumFunc, pid);
		if (auto hook = SetWindowsHookExW(WH_GETMESSAGE, HookCallBack, init_space::Info::hModule, idThread)) {
			PostThreadMessageW(idThread, WM_NULL, 0, 0);
		}
	}
}
