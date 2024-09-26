#include "DNAEvidence.h"
#include "../library/Console.hpp"
#include "Ghost/Esp/GhostEsp.hpp"
#include "Ghost/Ghost.hpp"


auto EvidenceController::GetDNAEvidence(EvidenceController* _this)
{
	GET_CLASS("Assembly-CSharp.dll", "EvidenceController", pClass) {
		try {
			return pClass->GetValue<DNAEvidence*>(static_cast<void*>(_this), 0x38);
		}
		catch (const std::exception& ex) {
			LOG_FMT_DEBUG(std::format("Error retrieving: ", ex.what()));
		}
	}
}

auto EvidenceController::InitOnce() -> void
{
	LOG_FMT_DEBUG("EvidenceController::InitOnce");

	GET_CLASS("Assembly-CSharp.dll", "EvidenceController", pClass) {
		//pClass->GetValue<DNAEvidence*>(static_cast<void*>(_this), pointer);
		// SpawnBoneDNAEvidence
		mSpawnBoneDNAEvidence = pClass->Get<IM>("SpawnBoneDNAEvidence")->Cast<void, EvidenceController*, Room*>();
	}

	if (mSpawnBoneDNAEvidence) {
		H::Install(mSpawnBoneDNAEvidence, HSpawnBoneDNAEvidence);
	}
}

inline auto UNITY_CALLING_CONVENTION EvidenceController::HSpawnBoneDNAEvidence(EvidenceController* _this, Room* room) -> void
{
	LOG_FMT_DEBUG("EvidenceController::HSpawnBoneDNAEvidence");
	H::Fcall(HSpawnBoneDNAEvidence, _this, room);
	auto pDNAEv = EvidenceController::GetDNAEvidence(_this);
	if (pDNAEv) {
		GhostAI::pDNA = pDNAEv;
	}
}

auto DNAEvidence::InitOnce() -> void
{
	LOG_FMT_DEBUG("DNAEvidence::InitOnce");
	EvidenceController::InitOnce();

	/*LOG_FMT_DEBUG("DNAEvidence::InitOnce");

	GET_CLASS("Assembly-CSharp.dll", "DNAEvidence", pClass) {
		//mBrightnessChangeValue = pClass->Get<IM>("BrightnessChangeValue")->Cast<void, GameGraphicsManager*, float>();
		//mUpdate = pClass->Get<IM>("Update")->Cast<void, GameGraphicsManager*>();
		mStart = pClass->Get<IM>("Start")->Cast<void, DNAEvidence*>();
	}

	//if (mStart) {
		//H::Install(mStart, HStart);
	//}
	*/
}

inline auto UNITY_CALLING_CONVENTION DNAEvidence::HStart(DNAEvidence* _this) -> void
{
	H::Fcall(HStart, _this);
	//_this->GetGameObject()->GetTransform()->GetPosition();

	LOG_FMT_DEBUG("DNAEvidence::HSpawn");
}

