#include "PlayerProperties.h"

auto PlayerProperties::GetInfo() const -> const GuiInfo& {
	return *new GuiInfo{ reinterpret_cast<const char*>(u8"(Players)"), reinterpret_cast<const char*>(u8" (Properties)"), true, false, false };
}
auto PlayerProperties::Draw() -> void { Feature::Draw(); }
auto PlayerProperties::Render() -> void {
	
}
auto PlayerProperties::Update() -> void {
	
}
auto PlayerProperties::Save(nlohmann::json& json) -> void { Feature::Save(json); }
auto PlayerProperties::Load(nlohmann::json& json) -> void { Feature::Load(json); }
auto PlayerProperties::GetInstance() -> PlayerProperties* {
	static PlayerProperties playerProperties;
	return &playerProperties;
}
