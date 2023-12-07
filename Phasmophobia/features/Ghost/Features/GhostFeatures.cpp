﻿#include "GhostFeatures.hpp"

#include "../Ghost.hpp"

auto GhostFeatures::GetInfo() const -> const GuiInfo& { return *new GuiInfo{ reinterpret_cast<const char*>(u8"鬼魂 (Ghost)"), reinterpret_cast<const char*>(u8"功能 (Features)"), true, false, false }; }
void GhostFeatures::Draw() { Feature::Draw(); }

// 为什么要用线程去执行?
// 防止栈引起连续的异常
void GhostFeatures::Render() {
	if (GhostAI::ghost) {
		if (ImGui::Button(U8(u8"现身 (Appear)"))) {
			std::thread([] {
				try {
					GhostAI::mAppear(GhostAI::ghost, -1);
				}
				catch (...) {}
			}).join();
		}

		if (ImGui::Button(U8(u8"隐身 (UnAppear)"))) {
			std::thread([] {
				try {
					GhostAI::mUnAppear(GhostAI::ghost);
				}
				catch (...) {}
			}).join();
		}

		if (ImGui::Button(U8(u8"闪现 (FlashAppear)"))) {
			std::thread([] {
				try {
					GhostAI::mFlashAppear(GhostAI::ghost);
				}
				catch (...) {}
			}).join();
		}

		if (ImGui::Button(U8(u8"强闪 (ForceFlash)"))) {
			std::thread([] {
				try {
					GhostAI::mForceFlash(GhostAI::ghost, true);
				}
				catch (...) {}
			}).join();
		}

		if (ImGui::Button(U8(u8"十字架 (CrucifixDelay)"))) {
			std::thread([] {
				try {
					GhostAI::mCrucifixDelay(GhostAI::ghost);
				}
				catch (...) {}
			}).join();
		}

		if (ImGui::Button(U8(u8"鬼能量距离检查 (JinnPowerDistanceCheck)"))) {
			std::thread([] {
				try {
					GhostAI::mJinnPowerDistanceCheck(GhostAI::ghost);
				}
				catch (...) {}
			}).join();
		}

		if (ImGui::Button(U8(u8"猎杀 (Hunting)"))) {
			std::thread([] {
				try {
					GhostAI::mHunting(GhostAI::ghost, true, 1000, nullptr);
				}
				catch (...) {}
			}).join();
		}

		if (ImGui::Button(U8(u8"注视最近玩家 (LookAtNearestPlayer)"))) {
			std::thread([] {
				try {
					GhostAI::mLookAtNearestPlayer(GhostAI::ghost);
				}
				catch (...) {}
			}).join();
		}

		if (ImGui::Button(U8(u8"传送到鬼房 (DelayTeleportToFavouriteRoom)"))) {
			std::thread([] {
				try {
					GhostAI::mDelayTeleportToFavouriteRoom(GhostAI::ghost, 0);
				}
				catch (...) {}
			}).join();
		}

		if (ImGui::Button(U8(u8"互动 (Interact)"))) {
			std::thread([] {
				try {
					GhostActivity::mInteract(GhostAI::ghost->ghostActivity);
				}
				catch (...) {}
			}).join();
		}

		if (ImGui::Button(U8(u8"随机门互动 (InteractWithARandomDoor)"))) {
			std::thread([] {
				try {
					GhostActivity::mInteractWithARandomDoor(GhostAI::ghost->ghostActivity);
				}
				catch (...) {}
			}).join();
		}

		if (ImGui::Button(U8(u8"随机物品互动 (InteractWithARandomProp)"))) {
			std::thread([] {
				try {
					GhostActivity::mInteractWithARandomProp(GhostAI::ghost->ghostActivity, true, true);
				}
				catch (...) {}
			}).join();
		}

		if (ImGui::Button(U8(u8"普通互动 (NormalInteraction)"))) {
			std::thread([] {
				try {
					GhostActivity::mNormalInteraction(GhostAI::ghost->ghostActivity, true);
				}
				catch (...) {}
			}).join();
		}

		if (ImGui::Button(U8(u8"双互动 (TwinInteraction)"))) {
			std::thread([] {
				try {
					GhostActivity::mTwinInteraction(GhostAI::ghost->ghostActivity, true);
				}
				catch (...) {}
			}).join();
		}

		if (ImGui::Button(U8(u8"检查最近电子设备 (CheckIfNearbyElectronicsAreOn)"))) {
			std::thread([] {
				try {
					GhostActivity::mCheckIfNearbyElectronicsAreOn(GhostAI::ghost->ghostActivity);
				}
				catch (...) {}
			}).join();
		}

		if (ImGui::Button(U8(u8"鬼魂能力 (GhostAbility)"))) {
			std::thread([] {
				try {
					GhostActivity::mGhostAbility(GhostAI::ghost->ghostActivity);
				}
				catch (...) {}
			}).join();
		}

		if (ImGui::Button(U8(u8"强制与道具交互 (ForceInteractWithProp)"))) {
			std::thread([] {
				try {
					GhostActivity::mForceInteractWithProp(GhostAI::ghost->ghostActivity, true);
				}
				catch (...) {}
			}).join();
		}
	} else {
		ImGui::Text(U8(u8"只有鬼存在时可用 (Only available when Jinn is present.)"));
	}
}
void GhostFeatures::Update() { Feature::Update(); }
void GhostFeatures::Save(nlohmann::json& json) { Feature::Save(json); }
void GhostFeatures::Load(nlohmann::json& json) { Feature::Load(json); }
auto GhostFeatures::GetInstance() -> GhostFeatures* {
	static GhostFeatures ghost_features;
	return &ghost_features;
}