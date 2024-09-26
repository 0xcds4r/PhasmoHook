#include "EMF.h"
#include "../library/Console.hpp"
#include "Ghost/Ghost.hpp"

auto EMF::InitOnce() -> void
{
	LOG_FMT_DEBUG("EMF::InitOnce");

	GET_CLASS("Assembly-CSharp.dll", "EMF", pClass) {
		//pClass->GetValue<DNAEvidence*>(static_cast<void*>(_this), pointer);
		// SpawnBoneDNAEvidence
		mUpdate = pClass->Get<IM>("Update")->Cast<void, EMF*>();
		//mOnEnable = pClass->Get<IM>("OnEnable")->Cast<void, EMF*>();
		//mOnDisable = pClass->Get<IM>("OnDisable")->Cast<void, EMF*>();
	}

	if (mUpdate) {
		H::Install(mUpdate, HUpdate);
	}

	//if (mOnEnable && mOnDisable) {
		//H::Install(mOnEnable, HOnEnable);
		//H::Install(mOnDisable, HOnDisable);
	//}
}

inline auto UNITY_CALLING_CONVENTION EMF::HUpdate(EMF* _this) -> void
{
	H::Fcall(HUpdate, _this);
	
	auto it = std::find_if(GhostAI::pEMF.begin(), GhostAI::pEMF.end(),
		[_this](const auto& entry) {
			return std::get<0>(entry) == _this; 
		});

	if (it == GhostAI::pEMF.end()) {
		auto now = std::chrono::steady_clock::now();
		GhostAI::pEMF.push_back(std::make_tuple(_this, now));
	}
}

inline auto UNITY_CALLING_CONVENTION EMF::HOnEnable(EMF* _this) -> void
{
	H::Fcall(HOnEnable, _this);
	LOG_FMT_DEBUG("EMF::HOnEnable");
	//GhostAI::pEMF.push_back(_this);
}

inline auto UNITY_CALLING_CONVENTION EMF::HOnDisable(EMF* _this) -> void
{
	H::Fcall(HOnDisable, _this);
	//auto it = std::find(GhostAI::pEMF.begin(), GhostAI::pEMF.end(), _this);
	/*if (it != GhostAI::pEMF.end()) {
		LOG_FMT_DEBUG("EMF::HOnDisable");
		GhostAI::pEMF.erase(it);
	}*/
}
