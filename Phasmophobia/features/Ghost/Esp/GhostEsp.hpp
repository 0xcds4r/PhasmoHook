#pragma once
#ifndef GHOST_ESP
#define GHOST_ESP
#include "../../../Init.h"

#define FLAG_ENABLE_ESP_LINES        0x01
#define FLAG_ENABLE_STATE_INFO       0x02
#define FLAG_ENABLE_BONE_WALLHACK    0x04

class GhostEsp : init_space::Feature {
public:
	GhostEsp() = default;
	~GhostEsp() = default;
	[[nodiscard]] auto GetInfo() const -> const GuiInfo & override;
	auto DrawGhostStateInfo(float distance) -> void;
	auto               Draw() -> void override;
	auto               Render() -> void override;
	auto               Update() -> void override;
	auto               Save(nlohmann::json& json) -> void override;
	auto               Load(nlohmann::json& json) -> void override;
	
	static auto GetInstance() -> GhostEsp*;

private:
	bool bEsp = false;
	bool bBoneWallHack = false;
	bool bStateInfo = true;

};
#endif
