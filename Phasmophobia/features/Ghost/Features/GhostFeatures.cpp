#include "GhostFeatures.hpp"

#include "../Ghost.hpp"

auto GhostFeatures::GetInfo() const -> const GuiInfo& { return *new GuiInfo{reinterpret_cast<const char*>(u8"(Ghost)"), reinterpret_cast<const char*>(u8"(Features)"), true, false, false}; }
auto GhostFeatures::Draw() -> void { Feature::Draw(); }

auto GhostFeatures::Render() -> void {
	if (GhostAI::ghost) {
		ImGui::TextColored(ImVec4(255.f, .0f, .0f, 1.f), U8(u8"(Not all features are functional.)"));
		if (ImGui::Button(U8(u8"(Appear)"))) { std::thread([] { try { GhostAI::mAppear(GhostAI::ghost, -1); } catch (...) {} }).join(); }

		if (ImGui::Button(U8(u8"(UnAppear)"))) { std::thread([] { try { GhostAI::mUnAppear(GhostAI::ghost); } catch (...) {} }).join(); }

		if (ImGui::Button(U8(u8"(FlashAppear)"))) { std::thread([] { try { GhostAI::mFlashAppear(GhostAI::ghost); } catch (...) {} }).join(); }

		if (ImGui::Button(U8(u8"(ForceFlash)"))) { std::thread([] { try { GhostAI::mForceFlash(GhostAI::ghost, true); } catch (...) {} }).join(); }

		if (ImGui::Button(U8(u8"(CrucifixDelay)"))) { std::thread([] { try { GhostAI::mCrucifixDelay(GhostAI::ghost); } catch (...) {} }).join(); }

		if (ImGui::Button(U8(u8"(JinnPowerDistanceCheck)"))) { std::thread([] { try { GhostAI::mJinnPowerDistanceCheck(GhostAI::ghost); } catch (...) {} }).join(); }

		//if (ImGui::Button(U8(u8"(Hunting)"))) { try { GhostAI::ghost->hunt = true;  GhostAI::mHunting(GhostAI::ghost, true, 1000, nullptr); } catch (...) {} }

		if (ImGui::Button(U8(u8"(LookAtNearestPlayer)"))) { std::thread([] { try { GhostAI::mLookAtNearestPlayer(GhostAI::ghost); } catch (...) {} }).join(); }

		if (ImGui::Button(U8(u8"(DelayTeleportToFavouriteRoom)"))) { std::thread([] { try { GhostAI::mDelayTeleportToFavouriteRoom(GhostAI::ghost, 0); } catch (...) {} }).join(); }

		if (ImGui::Button(U8(u8"(Interact)"))) { std::thread([] { try { GhostActivity::mInteract(GhostAI::ghost->ghostActivity); } catch (...) {} }).join(); }

		if (ImGui::Button(U8(u8"(InteractWithARandomDoor)"))) { std::thread([] { try { GhostActivity::mInteractWithARandomDoor(GhostAI::ghost->ghostActivity); } catch (...) {} }).join(); }

		if (ImGui::Button(U8(u8"(InteractWithARandomProp)"))) { std::thread([] { try { GhostActivity::mInteractWithARandomProp(GhostAI::ghost->ghostActivity, true, true); } catch (...) {} }).join(); }

		if (ImGui::Button(U8(u8"(NormalInteraction)"))) { std::thread([] { try { GhostActivity::mNormalInteraction(GhostAI::ghost->ghostActivity, true); } catch (...) {} }).join(); }

		if (ImGui::Button(U8(u8"(TwinInteraction)"))) { std::thread([] { try { GhostActivity::mTwinInteraction(GhostAI::ghost->ghostActivity, true); } catch (...) {} }).join(); }

		if (ImGui::Button(U8(u8"(CheckIfNearbyElectronicsAreOn)"))) { std::thread([] { try { GhostActivity::mCheckIfNearbyElectronicsAreOn(GhostAI::ghost->ghostActivity); } catch (...) {} }).join(); }

		if (ImGui::Button(U8(u8"(GhostAbility)"))) { std::thread([] { try { GhostActivity::mGhostAbility(GhostAI::ghost->ghostActivity); } catch (...) {} }).join(); }

		if (ImGui::Button(U8(u8"(ForceInteractWithProp)"))) { std::thread([] { try { GhostActivity::mForceInteractWithProp(GhostAI::ghost->ghostActivity, true); } catch (...) {} }).join(); }
	} else ImGui::Text(U8(u8"(Only available when Jinn is present.)"));
}

auto GhostFeatures::Update() -> void { Feature::Update(); }
auto GhostFeatures::Save(nlohmann::json& json) -> void { Feature::Save(json); }
auto GhostFeatures::Load(nlohmann::json& json) -> void { Feature::Load(json); }

auto GhostFeatures::GetInstance() -> GhostFeatures* {
	static GhostFeatures ghost_features;
	return &ghost_features;
}
