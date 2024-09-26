﻿#include "GhostProperties.h"
auto GhostProperties::GetInfo() const -> const GuiInfo& {
	return *new GuiInfo{ reinterpret_cast<const char*>(u8"(Ghost)"), reinterpret_cast<const char*>(u8"(Properties)"), true, true, true };
}
auto GhostProperties::Draw() -> void { Feature::Draw(); }
auto GhostProperties::Render() -> void { 
	if (GhostAI::ghost) {
		ImGui::Checkbox(U8(u8" (Modify ghost speed.)"), &gSpeedEnable);
		if (gSpeedEnable) ImGui::SliderFloat(U8(u8"(Ghost speed)"), &gSpeed, .0f, 50.f);
	} else ImGui::Text(U8(u8"(Only available when Jinn is present.)"));
}
auto GhostProperties::Update() -> void {
	if (gSpeedEnable) GhostAI::ghost->speed = gSpeed;
}
auto GhostProperties::Save(nlohmann::json& json) -> void { Feature::Save(json); }
auto GhostProperties::Load(nlohmann::json& json) -> void { Feature::Load(json); }
auto GhostProperties::GetInstance() -> GhostProperties* {
	static GhostProperties ghostProperties;
	return &ghostProperties;
}
