#include "Room.hpp"
#include "../../library/Console.hpp"

auto Room::InitOnce() -> void {
	LOG_FMT_DEBUG("Phasmohook initializing once LevelRoom..");

	if (const auto pClass = I::Get("Assembly-CSharp.dll")->Get("LevelRoom")) {
		mAwake = pClass->Get<IM>("Awake")->Cast<void, Room*>();
	}

	if (mAwake) {
		H::Install(mAwake, HAwake);
	}

	// ChangeFavouriteRoomTo
	LevelController::InitOnce();
}

auto LevelController::InitOnce() -> void {
	LOG_FMT_DEBUG("Phasmohook initializing once LevelController..");
	if (const auto pClass = I::Get("Assembly-CSharp.dll")->Get("LevelController")) {
		mGetFavGhostRoom = pClass->Get<IM>("get_favouriteGhostRoom")->Cast<Room*, LevelController*>();
		mAwake = pClass->Get<IM>("Awake")->Cast<void, LevelController*>();
	}

	if(mGetFavGhostRoom) H::Install(mGetFavGhostRoom, HGetFavGhostRoom);
	if(mAwake) H::Install(mAwake, HAwake);
}

inline auto UNITY_CALLING_CONVENTION LevelController::HGetFavGhostRoom(LevelController* _this) -> Room*
{
	return H::Fcall(HGetFavGhostRoom, _this);
}

inline auto UNITY_CALLING_CONVENTION LevelController::HAwake(LevelController* _this) -> void
{
	H::Fcall(HAwake, _this);
	gLevelController = _this;
	LOG_FMT_DEBUG(std::format("LevelController Address: {}\n", static_cast<void*>(_this)));
}

/*inline auto UNITY_CALLING_CONVENTION LevelController::HGetFavGhostRoom(LevelController* _this) -> void
{
	return auto UNITY_CALLING_CONVENTION();
}*/

auto Room::HAwake(Room* _this) -> void {
	std::lock_guard lock(Room::roomsMutex);
	rooms.push_back(_this);
	H::Fcall(HAwake, _this);
}
