#pragma once
#ifndef ROOM
#define ROOM
#include "../../Init.h"

enum class LocationType {
    InsideRoom,     // Represents being inside a room
    OutsideMap,     // Represents being outside the map
    Exterior,       // Represents an exterior location
    ExteriorRoom    // Represents an exterior room
};

enum class FloorType {
    Basement,       // Represents the basement
    FirstFloor,     // Represents the first floor
    SecondFloor,    // Represents the second floor
    ThirdFloor,     // Represents the third floor
    FourthFloor,    // Represents the fourth floor
    FifthFloor,     // Represents the fifth floor
    SixthFloor,     // Represents the sixth floor
    SeventhFloor,   // Represents the seventh floor
    EighthFloor,    // Represents the eighth floor
    NinthFloor,     // Represents the ninth floor
    TenthFloor      // Represents the tenth floor
};

#pragma pack(8)
class Room : public II::MonoBehaviour {
public:
	char space1[0x60];
    II::String* name;

    /*LocationType locationType;
    char space2[0x8];
    FloorType floorType;*/

    static inline II::String* GetOffsetValueStr(Room* _this, unsigned int pointer);

    static auto InitOnce() -> void;
	inline static std::vector<Room*> rooms{};
	inline static std::mutex         roomsMutex;

private:
	inline static IM::MethodPointer<void, Room*> mAwake{};
	inline static auto UNITY_CALLING_CONVENTION  HAwake(Room* _this) -> void;

    inline static IM::MethodPointer<void, Room*> mUpdate{};
    inline static auto UNITY_CALLING_CONVENTION  HUpdate(Room* _this) -> void;
};

class LevelController : public II::MonoBehaviour {
public:
    char skipData[0x108];
    static auto InitOnce() -> void;

    inline static IM::MethodPointer<Room*, LevelController*> mGetFavGhostRoom{};
    inline static IM::MethodPointer<void, LevelController*> mAwake{};
    inline static auto UNITY_CALLING_CONVENTION  HGetFavGhostRoom(LevelController* _this) -> Room*;
    inline static auto UNITY_CALLING_CONVENTION  HAwake(LevelController* _this) -> void;

    static inline LevelController* gLevelController;
};

#endif