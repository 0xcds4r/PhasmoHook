#pragma once
#include "../../Init.h"

class GameGraphicsManager : II::MonoBehaviour {
public:
	char skipDataGraphMgr[0x1DC];
	static auto InitOnce() -> void;

	inline static IM::MethodPointer<void, GameGraphicsManager*, float> mBrightnessChangeValue{};

	inline static IM::MethodPointer<void, GameGraphicsManager*> mUpdate;
	inline static auto UNITY_CALLING_CONVENTION  HUpdate(GameGraphicsManager* _this) -> void;
};

class RenderSettings : public II::MonoBehaviour
{
public:
	static auto InitOnce() -> void;
	
};

