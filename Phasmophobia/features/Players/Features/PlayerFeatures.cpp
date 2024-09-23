#include "PlayerFeatures.h"
#include "../../../GUI.h"
auto PlayerFeatures::GetInfo() const -> const GuiInfo& { return *new GuiInfo{reinterpret_cast<const char*>(u8"玩家 (Players)"), reinterpret_cast<const char*>(u8"功能 (Features)"), true, false, false}; }
auto PlayerFeatures::Draw() -> void { Feature::Draw(); }

auto PlayerFeatures::Render() -> void {
	/*std::lock_guard lock(Player::playersMutex);
	if (Player::players.empty()) {
		ImGui::Text(U8(u8"(There are currently no players present.)"));
		return;
	}
	for (const auto& player : Player::players) {
		try {
			if (player->photonView && player->photonView->ptr && player->photonView->ptr->name && player->levelRoom && player->levelRoom->name) {
				if (ImGui::BeginTabBar("PlayersList")) {
					if (ImGui::BeginTabItem(player->photonView->ptr->name->ToString().c_str())) {
						if (ImGui::Button(U8(u8"(Kill)"))) {
							try {
								Player::mStartKillingPlayer(player);
								Player::mKillPlayer(player, true);
								Player::mDeadRoomEffects(player);
							}
							catch (...) {}
						}

						if (ImGui::Button(U8(u8"(DeadRoomEffects)"))) {
							std::thread([&] {
								try { Player::mDeadRoomEffects(player); }
								catch (...) {}
							}).join();
						}
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
			}
		}
		catch (...) {}
	}*/

	if (ImGui::Checkbox("No Stamina", &bAntiStamina)) {
		Gui::AddNotify("No Stamina", bAntiStamina ? "Enabled" : "Disabled", 3.5f, ImColor(255, 255, 255, 255), ImColor(32, 68, 93, 220));
	}
}

auto PlayerFeatures::Update() -> void { Feature::Update(); }
auto PlayerFeatures::Save(nlohmann::json& json) -> void { Feature::Save(json); }
auto PlayerFeatures::Load(nlohmann::json& json) -> void { Feature::Load(json); }

auto PlayerFeatures::GetInstance() -> PlayerFeatures* {
	static PlayerFeatures player_features;
	return &player_features;
}
