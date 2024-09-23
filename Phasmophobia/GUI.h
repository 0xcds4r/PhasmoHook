#pragma once
#include "library/imgui/imgui.h"
#include <string>

class Gui {
public:
    Gui() = default;
    ~Gui() = default;

    // ������������� ������
    void ApplyStyles();

    // ��������� �����
    char ProcessInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // ������ ��������� ����
    void RenderMainWindow();

    static void AddNotify(const std::string& title, const std::string& message, float displayTime = 3.5f, ImColor textColor = ImColor(255, 255, 255, 255), ImColor bgColor = ImColor(40, 40, 40, 255));

    void updateLoop(std::chrono::steady_clock::time_point time);

    // ������ ���� ��������� ���
    void DoDrawFeatures();

private:
    ImVec4 HexToRGBA(const std::string& hex);
};
