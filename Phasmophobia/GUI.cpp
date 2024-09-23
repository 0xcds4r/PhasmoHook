

#include "Init.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"
#include "library/HotKey.hpp"
#include <fstream>
//#include <nlohmann/json.hpp>
#include <ranges>
#include <TlHelp32.h>
#include "GUI.h"
#include "library/Console.hpp"
#include "NotificationManager.h"

ImVec4 Gui::HexToRGBA(const std::string& hex) {
    int r, g, b, a;
    sscanf_s(hex.c_str(), "%02x%02x%02x%02x", &r, &g, &b, &a);
    return ImVec4(static_cast<float>(r) / 255, static_cast<float>(g) / 255, static_cast<float>(b) / 255, static_cast<float>(a) / 255);
}

void Gui::ApplyStyles() {
    LOG_FMT_DEBUG("Phasmohook applying gui styles..");

    auto& styles = ImGui::GetStyle();

    auto colors = styles.Colors;

    colors[ImGuiCol_Tab] = HexToRGBA("330000FF");              // Dark red background
    colors[ImGuiCol_TabHovered] = HexToRGBA("FF3333FF");       // Lighter neon red when hovered
    colors[ImGuiCol_TabActive] = HexToRGBA("FF0000FF");        // Neon red when active
    colors[ImGuiCol_TabUnfocused] = HexToRGBA("4D0000FF");     // Dark red when unfocused
    colors[ImGuiCol_TabUnfocusedActive] = HexToRGBA("800000FF"); // Slightly lighter red when active but unfocused

    colors[ImGuiCol_ScrollbarBg] = HexToRGBA("330000FF");              // Dark red background
    colors[ImGuiCol_ScrollbarGrab] = HexToRGBA("FF0000FF");            // Neon red scrollbar grab
    colors[ImGuiCol_ScrollbarGrabHovered] = HexToRGBA("FF3333FF");     // Lighter red when hovered
    colors[ImGuiCol_ScrollbarGrabActive] = HexToRGBA("FF4D4DFF");      // Light red when active

    colors[ImGuiCol_Border] = HexToRGBA("FF0000FF");         // Neon red
    colors[ImGuiCol_BorderShadow] = HexToRGBA("330000FF");   // Dark shadow
    colors[ImGuiCol_Button] = HexToRGBA("330000FF");         // Dark red background
    colors[ImGuiCol_ButtonActive] = HexToRGBA("FF3333FF");   // Neon red active
    colors[ImGuiCol_ButtonHovered] = HexToRGBA("FF4D4DFF");  // Lighter neon red on hover
    colors[ImGuiCol_CheckMark] = HexToRGBA("FF0000FF");      // Neon red checkmark
    colors[ImGuiCol_ChildBg] = HexToRGBA("000000AA");        // Dark semi-transparent background
    colors[ImGuiCol_DragDropTarget] = HexToRGBA("FF0000FF"); // Neon red drop target
    colors[ImGuiCol_FrameBg] = HexToRGBA("1A1A1ACC");        // Dark grey frame background
    colors[ImGuiCol_FrameBgActive] = HexToRGBA("FF0000FF");  // Neon red frame active
    colors[ImGuiCol_FrameBgHovered] = HexToRGBA("4D0000FF"); // Darker red on hover
    colors[ImGuiCol_Header] = HexToRGBA("4D0000FF");         // Header background
    colors[ImGuiCol_HeaderActive] = HexToRGBA("FF0000FF");   // Neon red header active
    colors[ImGuiCol_HeaderHovered] = HexToRGBA("990000FF");  // Darker header hover
    colors[ImGuiCol_MenuBarBg] = HexToRGBA("1A1A1AEE");      // Dark menu bar
    colors[ImGuiCol_ModalWindowDimBg] = HexToRGBA("000000CC"); // Dark transparent modal background
    colors[ImGuiCol_PopupBg] = HexToRGBA("1A1A1ADD");        // Popup background
    colors[ImGuiCol_ScrollbarBg] = HexToRGBA("33333399");    // Scrollbar background
    colors[ImGuiCol_ScrollbarGrab] = HexToRGBA("FF0000FF");  // Neon red scrollbar grab
    colors[ImGuiCol_Separator] = HexToRGBA("FF0000FF");      // Neon red separator
    colors[ImGuiCol_TitleBg] = HexToRGBA("330000FF");        // Dark red title background
    colors[ImGuiCol_TitleBgActive] = HexToRGBA("FF0000FF");  // Neon red active title background
    colors[ImGuiCol_WindowBg] = HexToRGBA("0D0D0DFF");       // Very dark background
    colors[ImGuiCol_Text] = HexToRGBA("E6E6E6FF");           // Bright white text
    colors[ImGuiCol_TextDisabled] = HexToRGBA("666666FF");   // Darker grey for disabled text
    colors[ImGuiCol_TextSelectedBg] = HexToRGBA("FF0000AA"); // Semi-transparent red highlight for selected text

    styles.Alpha = 1.0;
    styles.AntiAliasedFill = true;
    styles.AntiAliasedLines = true;
    styles.AntiAliasedLinesUseTex = true;
    styles.ButtonTextAlign = ImVec2(0.5, 0.5);
    styles.CellPadding = ImVec2(4.0, 2.0);
    styles.ChildBorderSize = 1.0;
    styles.ChildRounding = 0.0;
    styles.FrameBorderSize = 1.0;
    styles.FramePadding = ImVec2(5.0, 4.0);
    styles.FrameRounding = 0.0;
    styles.GrabMinSize = 12.0;
    styles.GrabRounding = 0.0;
    styles.IndentSpacing = 22.0;
    styles.ItemSpacing = ImVec2(10.0, 6.0);
    styles.ItemInnerSpacing = ImVec2(6.0, 6.0);
    styles.PopupBorderSize = 1.0;
    styles.PopupRounding = 2.0;
    styles.ScrollbarSize = 14.0;
    styles.ScrollbarRounding = 0.0;
    styles.TabBorderSize = 1.0;
    styles.TabRounding = 0.0;
    styles.WindowBorderSize = 1.0;
    styles.WindowRounding = 0.0;
    styles.WindowPadding = ImVec2(10.0, 10.0);
    styles.WindowTitleAlign = ImVec2(0.5, 0.5);
}

extern auto ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
char Gui::ProcessInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HotKey::PotMsg(msg);

    switch (msg) {
    case WM_KEYUP:
        if (wParam == VK_SHIFT) {
            init_space::Info::shiftPressed = false;
        }
        break;
    case WM_KEYDOWN:
        if (wParam == VK_SHIFT) {
            init_space::Info::shiftPressed = true;
        }
        if (wParam == VK_RETURN) {
            init_space::Info::mainShow = !init_space::Info::mainShow;
            ShowCursor(init_space::Info::mainShow);
        }
        break;
    case WM_CLOSE:
        if (MessageBox(nullptr, L"Exit?", L"Confirmation", MB_YESNO | MB_ICONQUESTION) == IDYES) {
            exit(0);
        }
        break;
    }

    if (init_space::Info::mainShow) {
        ClipCursor(nullptr);
        return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
    }

    ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
    return 1;
}

NotificationManager notificationManager;

void Gui::AddNotify(const std::string& title, const std::string& message, float displayTime, ImColor textColor, ImColor bgColor) {
    notificationManager.addNotification(title, message, displayTime, 10.0f, textColor, bgColor);
}

std::chrono::steady_clock::time_point lastFrameTime;
void Gui::updateLoop(std::chrono::steady_clock::time_point time) {
    lastFrameTime = time;
}

void Gui::DoDrawFeatures() 
{
    auto currentFrameTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentFrameTime - lastFrameTime).count();
    lastFrameTime = currentFrameTime;

    ImGui::GetIO().DeltaTime = deltaTime;

    static bool once = false;

    if (!once) {
        Gui::AddNotify("Phasmohook", "Successfully injected!\n\nPress 'Enter' key for Open Menu!", 3.5f, ImColor(255, 255, 255, 255), ImColor(32, 68, 93, 220));
        once = true;
    }
    
    notificationManager.render();

    for (const auto& feature : init_space::Feature::features | std::views::values) {
        for (const auto func : feature) {
            if (func->GetInfo().needDraw) {
                func->Draw();
            }
        }
    }
}

void Gui::RenderMainWindow() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 screenCenter(io.DisplaySize.x / 2.0f, io.DisplaySize.y / 2.0f);
    ImVec2 windowSize(600, 500);
    ImVec2 windowPos(screenCenter.x - windowSize.x / 2.0f, screenCenter.y - windowSize.y / 2.0f);

    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.5f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.8f));

    if (ImGui::Begin("Phasmohook", &init_space::Info::mainShow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse)) {
        //ImGui::TextColored(ImVec4(0.7f, 0.9f, 0.7f, 1.0f), "Configuration");
        

        /*if (ImGui::Button("Save")) {
            if (std::ofstream o(".\\cfg.json"); o) {
                nlohmann::json js;
                for (const auto& _features : init_space::Feature::features | std::views::values) {
                    for (const auto func : _features) {
                        func->Save(js);
                    }
                }
                o << js;
            }
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Save settings to cfg.json");

        ImGui::SameLine();
        if (ImGui::Button("Load")) {
            if (std::ifstream i(".\\cfg.json"); i) {
                auto js = nlohmann::json::parse(i);
                for (const auto& _features : init_space::Feature::features | std::views::values) {
                    for (const auto func : _features) {
                        try {
                            func->Load(js);
                        }
                        catch (...) {}
                    }
                }
            }
        }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Load settings from cfg.json");

        if (ImGui::Button("test notify")) {
            Gui::AddNotify("Test notify", "Phasmohook Test Notify!", 5.5f);
        }*/

        //ImGui::Separator();
        ImGui::TextColored(ImVec4(0.7f, 0.9f, 0.7f, 1.0f), "Cheats");
        ImGui::Separator();

        for (const auto& [name, _features] : init_space::Feature::features) {
            for (const auto func : _features) {
                func->Render();
            }
        }

        ImGui::End();
    }

    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(1);
}