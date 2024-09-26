#include "RenderSettings.h"
#include "../../library/Console.hpp"

auto RenderSettings::InitOnce() -> void
{
	LOG_FMT_DEBUG("RenderSettings::InitOnce");
	GameGraphicsManager::InitOnce();
}

auto GameGraphicsManager::InitOnce() -> void
{
	/*LOG_FMT_DEBUG("GameGraphicsManager::InitOnce");

	GET_CLASS("Assembly-CSharp.dll", "GameGraphicsManager", pClass) {
		mBrightnessChangeValue = pClass->Get<IM>("BrightnessChangeValue")->Cast<void, GameGraphicsManager*, float>();
		mUpdate = pClass->Get<IM>("Update")->Cast<void, GameGraphicsManager*>();
	}

	if (mUpdate) {
		H::Install(mUpdate, HUpdate);
	}*/
}

inline auto UNITY_CALLING_CONVENTION GameGraphicsManager::HUpdate(GameGraphicsManager* _this) -> void
{
	H::Fcall(HUpdate, _this);
}
