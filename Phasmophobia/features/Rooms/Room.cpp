#include "Room.hpp"
#include "../../library/Console.hpp"
#include "../Ghost/Ghost.hpp"

II::String* Room::GetOffsetValueStr(Room* _this, unsigned int pointer)
{
	GET_CLASS("Assembly-CSharp.dll", "LevelRoom", pClass) {
		try {
			return pClass->GetValue<II::String*>(static_cast<void*>(_this), pointer);
		}
		catch (const std::exception& ex) {
			LOG_FMT_DEBUG(std::format("Error retrieving: ", ex.what()));
		}
	}
}

auto Room::InitOnce() -> void {
	LOG_FMT_DEBUG("Phasmohook initializing once LevelRoom..");

	GET_CLASS("Assembly-CSharp.dll", "LevelRoom", pClass) {
		mAwake = pClass->Get<IM>("Awake")->Cast<void, Room*>();
		mUpdate = pClass->Get<IM>("Update")->Cast<void, Room*>();
	}

	if (mAwake) {
		H::Install(mAwake, HAwake);
	}

	if (mUpdate) {
		H::Install(mUpdate, HUpdate);
	}

	// ChangeFavouriteRoomTo
	LevelController::InitOnce();
}

auto LevelController::InitOnce() -> void {
	LOG_FMT_DEBUG("Phasmohook initializing once LevelController..");

	GET_CLASS("Assembly-CSharp.dll", "LevelController", pClass) {
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

inline auto UNITY_CALLING_CONVENTION Room::HUpdate(Room* _this) -> void
{
	H::Fcall(HUpdate, _this);

	if (GhostAI::ghost) 
	{
		if (LevelController::gLevelController) {
			Room* favRoom = LevelController::HGetFavGhostRoom(LevelController::gLevelController);
			if (favRoom && static_cast<void*>(_this) == static_cast<void*>(favRoom)) {
				std::string szFavRoom = std::format("{}", Room::GetOffsetValueStr(favRoom, 0x60)->ToString());
				//std::cout << szFavRoom.c_str() << std::endl;
				if(szFavRoom.size() != 0 && szFavRoom.length() > 0)
					GhostAI::UpdateFavouriteRoom(szFavRoom);
			}
		}
	}
}
