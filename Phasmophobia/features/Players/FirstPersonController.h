#pragma once
#include "../../Init.h"

#pragma pack(8)
class FirstPersonController : public II::MonoBehaviour
{
public:
    // Offset: 0x20
    bool canMove;              

    // Offset: 0x21
    bool canTurn;            

    // Offset: 0x22
    bool m_IsSprinting;      

    // Offset: 0x23
    bool m_IsMoving;          

    // Offset: 0x24
    bool canSprint;         

    // Offset: 0x25 - 0x27
    char pad1[0x28 - 0x25];     

    // Offset: 0x28
    float m_RunstepLenghten;    

    // Offset: 0x2C
    float m_StickToGroundForce; 

    // Offset: 0x30
    float m_GravityMultiplier; 

    // Offset: 0x34 - 0x37
    char pad2[0x38 - 0x34];    

    // Offset: 0x38
    char m_MouseLook[8];       

    // Offset: 0x40
    bool m_UseFovKick;         

    // Offset: 0x41 - 0x47
    char pad3[0x48 - 0x41];     

    // Offset: 0x48
    char m_FovKick[8];         

    // Offset: 0x50
    bool m_UseHeadBob;         

    // Offset: 0x51
    bool _toggleSprint;       

    // Offset: 0x52 - 0x57
    char pad4[0x58 - 0x52];     

    // Offset: 0x58
    char m_HeadBob[8];         

    // Offset: 0x60
    float m_StepInterval;       

    // Offset: 0x64 - 0x67
    char pad5[0x68 - 0x64];     

    // Offset: 0x68
    char m_Camera[8];           

    // Offset: 0x70
    char m_Input[8];           

    // Offset: 0x78
    char m_MoveDir[12];        

    // Offset: 0x84 - 0x87
    char pad6[0x88 - 0x84];     

    // Offset: 0x88
    char m_CharacterController[8]; 

    // Offset: 0x90
    int walkAlongLayers;        

    // Offset: 0x94
    bool m_Crouching;          

    // Offset: 0x95 - 0x97
    char pad7[0x98 - 0x95];    

    // Offset: 0x98
    float currentSpeed;        

    // Offset: 0x9C
    char desiredMove[12];       

    // Offset: 0xA8
    char hitInfo[56];           

    // Offset: 0xD8
    char OnStartSprint[8];     

    // Offset: 0xE0
    char OnStartWalk[8];       

	static auto InitOnce() -> void;

    inline static auto GetOffsetValue(FirstPersonController* _this, const std::string& name);

	inline static IM::MethodPointer<void, FirstPersonController*> mUpdate{};

	inline static auto UNITY_CALLING_CONVENTION  HUpdate(FirstPersonController* _this) -> void;
};

