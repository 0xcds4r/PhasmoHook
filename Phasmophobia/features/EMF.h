#pragma once

#include "../Init.h"

enum class EMFGhostAction {
    GhostInteraction,   // (2) Ghost Interaction
    GhostThrowing,      // (3) Ghost Throwing
    GhostAppeared,      // (4) Ghost Appeared
    GhostEvidence       // (5) Ghost Evidence - DO NOT USE
};

enum class EMFGhostActionType {
    interaction,
    objectMoved,
    windowKnock,
    midGhostWrite,
    breakerSwitch,
    planchette,
    rockingChair,
    generalIntEffect,
    lightSwitched,
    soundEffect,
    crucifixFire
};

class EMF : public II::MonoBehaviour {
public:
    char padding[0x30]; // Skip to offset 0x30, which is where ghostAction starts
    EMFGhostAction ghostAction;
    EMFGhostActionType ghostActionType;
    float placeholder_3;
public:
    static auto InitOnce() -> void;

    inline static IM::MethodPointer<void, EMF*> mUpdate;

    inline static IM::MethodPointer<void, EMF*> mOnEnable;
    inline static IM::MethodPointer<void, EMF*> mOnDisable;

    inline static auto UNITY_CALLING_CONVENTION  HUpdate(EMF* _this) -> void;

    inline static auto UNITY_CALLING_CONVENTION  HOnEnable(EMF* _this) -> void;
    inline static auto UNITY_CALLING_CONVENTION  HOnDisable(EMF* _this) -> void;
};