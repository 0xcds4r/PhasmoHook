﻿#pragma once
#ifndef INIT_SPACE
#define INIT_SPACE
#include "library/json.hpp"
#include "library/detours/HookManager.h"
#include "library/imgui/imgui.h"
#include "library/imgui/imgui_internal.h"
#include "library/UnityResolve.hpp"
#include "library/imgui/implot.h"
#include "library/magic_enum/magic_enum_all.hpp"

#include <windows.h>
#include <future>
#include <thread>
#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <mutex>
#include <vector>
#include <cstdint>
#include <map>

#define FOR_EACH_COMPONENT(gPtr, componentType, dllName, componentName, componentVar) \
    for (const auto componentVar : (gPtr)->GetGameObject()->GetComponentsInChildren<componentType*>(I::Get(dllName)->Get(componentName)))

#define GET_CLASS(dllName, className, classVar) \
    if (const auto classVar = I::Get(dllName)->Get(className))

#define U8(X) reinterpret_cast<const char*>(X)
using I = UnityResolve;
using IM = UnityResolve::Method;
using IC = UnityResolve::Class;
using IT = UnityResolve::Type;
using IF = UnityResolve::Field;
using IA = UnityResolve::Assembly;
using II = UnityResolve::UnityType;
using H = HookManager;

namespace init_space {
    class Info {
    public:
        inline static HMODULE hModule;
        inline static bool mainShow;
        inline static bool imGuiInit;
        inline static bool tipsShow;
        inline static bool shiftPressed;
        inline static LONG h;
        inline static LONG w;
        inline static bool show;
    };

    class Feature {
    public:
        Feature(const Feature&)                = delete;
        auto operator=(const Feature&) -> void = delete;

        struct GuiInfo {
            std::string tableName;
            std::string groupName;
            bool        needGroup;
            bool        needUpdate;
            bool        needDraw;
        };

        virtual auto GetInfo() const -> const GuiInfo& = 0;
        virtual auto Draw() -> void { };
        virtual auto Render() -> void {};
        virtual auto Update() -> void {};
        virtual auto Save(nlohmann::json& json) -> void {};
        virtual auto Load(nlohmann::json& json) -> void {};

        inline static std::unordered_map<std::string, std::vector<Feature*>> features;
        static auto Init() -> void;

    protected:
        ~Feature() = default;
        Feature()  = default;
    };

}

#include "ImGuiEX.h"
#include "DrawMath.h"
#endif
