#include "Player.hpp"

#include "../Ghost/Ghost.hpp"

#include <codecvt>
#include <locale>
#include <string>
#include "../../library/Console.hpp"

inline auto Player::GetOffsetValue(Player* _this, const std::string& name)
{
	if (const auto pClass = I::Get("Assembly-CSharp.dll")->Get("Player")) {
		try {
			return pClass->GetValue<float>(static_cast<void*>(_this), name);
		}
		catch (const std::exception& ex) {
			std::cerr << "Error retrieving " << name.c_str() << ": " << ex.what() << std::endl;
		}
	}
}

auto Player::InitOnce() -> void {
	LOG_FMT_DEBUG("Phasmohook initializing once Player..");

	if (const auto pClass = I::Get("Assembly-CSharp.dll")->Get("Player")) {
		mAwake              = pClass->Get<IM>("Awake")->Cast<void, Player*>();
		mOnDestroy          = pClass->Get<IM>("OnDestroy")->Cast<void, Player*>();
		mKillPlayer         = pClass->Get<IM>("KillPlayer")->Cast<void, Player*, bool>();
		mStartKillingPlayer = pClass->Get<IM>("StartKillingPlayer")->Cast<void, Player*>();
		mDead               = pClass->Get<IM>("Dead")->Cast<void, Player*, bool, void*>();
		mDeadRoomEffects    = pClass->Get<IM>("DeadRoomEffects")->Cast<void, Player*>();
		mTeleport           = pClass->Get<IM>("Teleport");
		mSetPlayerSpeed		= pClass->Get<IM>("SetPlayerSpeed")->Cast<void, Player*, float>();
		mUpdate				= pClass->Get<IM>("Update")->Cast<void, Player*>();

		//pClass->PrintFieldNames();
		//for (const auto& field : pClass->fields) {
		//	std::string szFieldName(field->name);
		//	std::wstring wideName = Utf8ToWide(field->name);
		//	std::wcout << L"Field Name: " << ConvertFromWideToUtf8(wideName).c_str() << std::endl;
		//	//std::cout << "Field Name: " << field->name << std::endl;
		//}
	}

	if (mAwake && mOnDestroy) {
		H::Install(mAwake, HAwake);
		H::Install(mOnDestroy, HOnDestroy);
	}

	if (mSetPlayerSpeed) {
		H::Install(mSetPlayerSpeed, HSetPlayerSpeed);
	}

	if (mUpdate) {
		H::Install(mUpdate, HUpdate);
	}
}

// ApplySprintMode
auto UNITY_CALLING_CONVENTION Player::HUpdate(Player* _this) -> void {
	H::Fcall(HUpdate, _this);
	//if (init_space::Info::shiftPressed) {

		/*
		if (isWalking == false) {
    speed = 1.60000002;
  }
  else {
    speed = 1.20000005;
  }
		*/
		// Player::HSetPlayerSpeed(_this, init_space::Info::shiftPressed ? 1.20000005f : 1.40000005);
	//}
}

auto UNITY_CALLING_CONVENTION Player::HSetPlayerSpeed(Player* _this, float speed) -> void {
	//std::cout << "Player::HSetPlayerSpeed -> " + std::to_string(speed) << std::endl;
	//LOG_DEBUG(std::format("Player::HSetPlayerSpeed: {} {}\n", static_cast<void*>(_this), speed));
	// speed hack?
	H::Fcall(HSetPlayerSpeed, _this, speed);
}

auto UNITY_CALLING_CONVENTION Player::HAwake(Player* _this) -> void {
	std::lock_guard lock(playersMutex);
	players.push_back(_this);
	H::Fcall(HAwake, _this);
}

auto UNITY_CALLING_CONVENTION Player::HOnDestroy(Player* _this) -> void {
	std::lock_guard lock(playersMutex);
	if (const auto it = std::ranges::find(players, _this); it != players.end()) players.erase(it);
	if (players.empty()) {
		GhostAI::ghost = nullptr;
		Room::rooms.clear();
	}
	H::Fcall(HAwake, _this);
}
