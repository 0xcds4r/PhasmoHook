#pragma once

#include "../Init.h"
#include "Rooms/Room.hpp"

class EvidenceController : public II::MonoBehaviour {
public:
    void* data;
public:
    static auto GetDNAEvidence(EvidenceController* _this);
    static auto InitOnce() -> void;

    inline static IM::MethodPointer<void, EvidenceController*, Room*> mSpawnBoneDNAEvidence;
    inline static auto UNITY_CALLING_CONVENTION HSpawnBoneDNAEvidence(EvidenceController* _this, Room* room) -> void;
};

class DNAEvidence : public II::MonoBehaviourPun
{
public:
    /*void* photonInteract;    // Pointer to PhotonObjectInteract
    II::Transform* losTarget;                     // Pointer to Transform
    II::Rigidbody* rigid;                         // Pointer to Rigidbody
    void* meshFilter;                   // Pointer to MeshFilter
    void* meshCollider;               // Pointer to MeshCollider
    Room* room;                          // Pointer to LevelRoom

    // Bones
    std::vector<II::Mesh*> boneMeshes;            // Vector of pointers to Mesh
    void* collectable;                 // Pointer to Collectable
    */
    void* data;
public:
	static auto InitOnce() -> void;

	inline static IM::MethodPointer<void, DNAEvidence*> mStart;
	inline static auto UNITY_CALLING_CONVENTION  HStart(DNAEvidence* _this) -> void;
};

