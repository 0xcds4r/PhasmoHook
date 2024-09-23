#pragma once
#include "../../Init.h"
#include "FirstPersonController.h"

class PlayerStamina : public II::MonoBehaviour
{
public:
	char player_stamina_data[0x60];
public:
	static auto InitOnce() -> void;
	inline static IM::MethodPointer<void, PlayerStamina*, float> mPreventStaminaDrainForTime{};
	inline static IM::MethodPointer<void, PlayerStamina*> mUpdate;
	inline static auto UNITY_CALLING_CONVENTION  HUpdate(PlayerStamina* _this) -> void;
};
//static_assert(sizeof(PlayerStamina) == 0x80, "PlayerStamina size is incorrect");

class PCStamina : public II::MonoBehaviour {
public:
	char pc_stamina_data[0x68];
public:
	static auto InitOnce() -> void;

	inline static IM::MethodPointer<void, PCStamina*> mUpdate;
	inline static auto UNITY_CALLING_CONVENTION  HUpdate(PCStamina* _this) -> void;
};
//static_assert(sizeof(PCStamina) == 0x88, "PCStamina size is incorrect");

//Size of PlayerStamina : 128 bytes
//Size of PCStamina : 136 bytes
