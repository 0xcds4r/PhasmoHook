#include "PlayerStamina.hpp"
#include "Features/PlayerFeatures.h"
#include "../../library/Console.hpp"

auto PlayerStamina::InitOnce() -> void {
	LOG_FMT_DEBUG("Phasmohook initializing once PlayerStamina..");

	GET_CLASS("Assembly-CSharp.dll", "PlayerStamina", pClass) {
		mUpdate = pClass->Get<IM>("Update")->Cast<void, PlayerStamina*>();
	}

	if (mUpdate) {
		H::Install(mUpdate, HUpdate);
	}

	//std::cout << "Size of PlayerStamina: " << sizeof(PlayerStamina) << " bytes" << std::endl;
}

inline auto UNITY_CALLING_CONVENTION PlayerStamina::HUpdate(PlayerStamina* _this) -> void
{
	if (PlayerFeatures::bAntiStamina)
		return;

	H::Fcall(HUpdate, _this);
}

auto PCStamina::InitOnce() -> void {
	LOG_FMT_DEBUG("Phasmohook initializing once PCStamina..");

	GET_CLASS("Assembly-CSharp.dll", "PCStamina", pClass) {
		mUpdate = pClass->Get<IM>("Update")->Cast<void, PCStamina*>();
	}

	if (mUpdate) {
		H::Install(mUpdate, HUpdate);
	}

	//PlayerStamina::InitOnce(); // useless
	//std::cout << "Size of PCStamina: " << sizeof(PCStamina) << " bytes" << std::endl;
}

inline auto UNITY_CALLING_CONVENTION PCStamina::HUpdate(PCStamina* _this) -> void
{
	if (PlayerFeatures::bAntiStamina) // good, but no footsteps sounds
		return;
	H::Fcall(HUpdate, _this);
}
