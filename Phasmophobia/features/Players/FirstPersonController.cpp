#include "FirstPersonController.h"
#include "Features/PlayerFeatures.h"
#include "../../library/Console.hpp"


auto FirstPersonController::InitOnce() -> void {
	LOG_FMT_DEBUG("Phasmohook initializing once FirstPersonController..");

	GET_CLASS("Assembly-CSharp-firstpass.dll", "FirstPersonController", pClass) {
		mUpdate = pClass->Get<IM>("Update")->Cast<void, FirstPersonController*>();
	}

	if (mUpdate) {
		H::Install(mUpdate, HUpdate);
	}
}

inline auto FirstPersonController::GetOffsetValue(FirstPersonController* _this, const std::string& name) 
{
	GET_CLASS("Assembly-CSharp-firstpass.dll", "FirstPersonController", pClass) {
		try {
			return pClass->GetValue<float>(static_cast<void*>(_this), name);
		}
		catch (const std::exception& ex) {
			LOG_FMT_DEBUG("Error retrieving -> {} {}", name.c_str(), ex.what());
		}
	}
}

inline auto UNITY_CALLING_CONVENTION FirstPersonController::HUpdate(FirstPersonController* _this) -> void
{
	//std::cout << "FirstPersonController::HUpdate calling" << std::endl;

	//std::cout << "Gravity is -> " << _this->m_GravityMultiplier << std::endl;

	// legit anti stamina?

	//if (PlayerFeatures::bAntiStamina)
	//{
		//_this->canSprint = true;
		//_this->canTurn = true;
		//_this->canMove = true;
	//}
	//if (init_space::Info::shiftPressed) {
		//_this->m_IsSprinting = true;
		//_this->canSprint = true;
		//_this->canTurn = true;
		//_this->canMove = true;
		//std::cout << "Current speed is -> " << _this->currentSpeed << std::endl;
	//}
	

	/*if (const auto pClass = I::Get("Assembly-CSharp-firstpass.dll")->Get("FirstPersonController")) {
		try {
			float curSpeed = pClass->GetValue<float>(static_cast<void*>(_this), "currentSpeed");
			std::cout << "Current speed is -> " << curSpeed << std::endl;
		}
		catch (const std::exception& ex) {
			std::cerr << "Error retrieving currentSpeed: " << ex.what() << std::endl;
		}
	}*/
	H::Fcall(HUpdate, _this);
}
