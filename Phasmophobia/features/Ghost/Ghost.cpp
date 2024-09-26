#include "Ghost.hpp"

#include "../../library/Console.hpp"

inline auto GhostAI::GetOffsetValue(GhostAI* _this, const std::string& name)
{
	GET_CLASS("Assembly-CSharp.dll", "GhostAI", pClass) {
		try {
			return pClass->GetValue<float>(static_cast<void*>(_this), name);
		}
		catch (const std::exception& ex) {
			LOG_FMT_DEBUG("Error retrieving -> {} {}", name.c_str(), ex.what());
		}
	}
}

Room* GhostAI::GetGhostCurrentRoom(GhostAI* _this)
{
	GET_CLASS("Assembly-CSharp.dll", "GhostAI", pClass) {
		void* pGhostInfo = pClass->GetValue<void*>(static_cast<void*>(_this), 0x38);

		GET_CLASS("Assembly-CSharp.dll", "GhostInfo", pGhostInfoClass) {
			return pGhostInfoClass->GetValue<Room*>(pGhostInfo, 0x70);
		}
	}

	return nullptr;
}

// 0x38 + 0x70

auto GhostAI::InitOnce() -> void {
	LOG_FMT_DEBUG("Phasmohook initializing once GhostAI..");

	if (const auto pClass = I::Get("Assembly-CSharp.dll")->Get("GhostAI")) {
		mAwake = pClass->Get<IM>("Awake")->Cast<void, GhostAI*>();
		mAppear = pClass->Get<IM>("Appear")->Cast<void, GhostAI*, int>();
		mCrucifixDelay = pClass->Get<IM>("CrucifixDelay")->Cast<void, GhostAI*>();
		mFlashAppear = pClass->Get<IM>("FlashAppear")->Cast<void, GhostAI*>();
		mDelayTeleportToFavouriteRoom = pClass->Get<IM>("DelayTeleportToFavouriteRoom")->Cast<void, GhostAI*, float>();
		mForceFlash = pClass->Get<IM>("ForceFlash")->Cast<void, GhostAI*, bool>();
		mHunting = pClass->Get<IM>("Hunting")->Cast<void, GhostAI*, bool, int, PhotonMessageInfo*>();
		mJinnPowerDistanceCheck = pClass->Get<IM>("JinnPowerDistanceCheck")->Cast<void, GhostAI*>();
		mUnAppear = pClass->Get<IM>("UnAppear")->Cast<void, GhostAI*>();
		mLookAtNearestPlayer = pClass->Get<IM>("LookAtNearestPlayer")->Cast<void, GhostAI*>();
		mLookAtPlayer = pClass->Get<IM>("LookAtPlayer")->Cast<void, GhostAI*, Player*>();
	}

	if (const auto pClass = I::Get("Assembly-CSharp.dll")->Get("GhostActivity")) {
		GhostActivity::mInteract = pClass->Get<IM>("Interact")->Cast<void, GhostActivity*>();
		GhostActivity::mInteractWithARandomDoor = pClass->Get<IM>("InteractWithARandomDoor")->Cast<bool, GhostActivity*>();
		GhostActivity::mInteractWithARandomProp = pClass->Get<IM>("InteractWithARandomProp")->Cast<void, GhostActivity*, bool, bool>();
		GhostActivity::mNormalInteraction = pClass->Get<IM>("NormalInteraction")->Cast<void, GhostActivity*, bool>();
		GhostActivity::mTwinInteraction = pClass->Get<IM>("TwinInteraction")->Cast<void*, GhostActivity*, bool>();
		GhostActivity::mCheckIfNearbyElectronicsAreOn = pClass->Get<IM>("CheckIfNearbyElectronicsAreOn")->Cast<void, GhostActivity*>();
		GhostActivity::mGhostAbility = pClass->Get<IM>("GhostAbility")->Cast<void, GhostActivity*>();
		GhostActivity::mForceInteractWithProp = pClass->Get<IM>("ForceInteractWithProp")->Cast<void, GhostActivity*, bool>();
	}

	if (mAwake) {
		H::Install(mAwake, HAwake);
	}
}

void GhostAI::UpdateFavouriteRoom(const std::string& roomName)
{
	GhostAI::favRoom = roomName;
}

void GhostAI::UpdateCurrentRoom(const std::string& roomName)
{
	GhostAI::currentRoom = roomName;
}

auto GhostAI::HAwake(GhostAI* _this) -> void {
	GhostAI::pEMF.clear();
	GhostAI::pDNA = nullptr;
	ghost = _this;
	//LOG_DEBUG(std::format("GhostAI Address: {}\n", static_cast<void*>(_this)));
	H::Fcall(HAwake, _this);
}
