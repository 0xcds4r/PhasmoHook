#pragma once
#ifndef PLAYER
#define PLAYER
#include "../../Init.h"
#include "../Rooms/Room.hpp"

#pragma pack(8)
class Player : public II::MonoBehaviour {
public:
	struct : MonoBehaviour {
		char space1[0x50];
		struct : MonoBehaviour {
			II::String* name;
		}*ptr;
	}*photonView; // 0x20
	bool dead; // 0x28
	bool beingHunted; // 0x29
	char space2[0x29]; // 0x58
	Room* levelRoom; // 0x60
	char space3[0x70];
	struct : MonoBehaviour {
		char space1[0x110];
		struct TMP_Text {
			char space1[0xE0];
			II::String* text;
		};

		struct TextMeshProUGUI : TMP_Text {

		}*textMeshProUgui;
	}*journalController;

	static inline auto GetOffsetValue(Player* _this, const std::string& name);

	static auto InitOnce() -> void;
	inline static std::vector<Player*> players{};
	inline static std::mutex           playersMutex;

	inline static IM* mTeleport{};

	inline static IM::MethodPointer<void, Player*> mStartKillingPlayer{};
	inline static IM::MethodPointer<void, Player*, bool> mKillPlayer{};
	inline static IM::MethodPointer<void, Player*, bool, void*> mDead{};
	inline static IM::MethodPointer<void, Player*> mDeadRoomEffects{};

	
	inline static IM::MethodPointer<void, Player*> mUpdate;
	inline static auto UNITY_CALLING_CONVENTION  HUpdate(Player* _this) -> void;


	inline static IM::MethodPointer<void, Player*, float> mSetPlayerSpeed;
	inline static auto UNITY_CALLING_CONVENTION  HSetPlayerSpeed(Player* _this, float speed) -> void;

private:
	inline static IM::MethodPointer<void, Player*> mAwake{};
	inline static auto UNITY_CALLING_CONVENTION  HAwake(Player* _this) -> void;
	inline static IM::MethodPointer<void, Player*> mOnDestroy{};
	inline static auto UNITY_CALLING_CONVENTION  HOnDestroy(Player* _this) -> void;
};

#endif