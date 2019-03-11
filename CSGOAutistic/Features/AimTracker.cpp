#include "AimTracker.h"
#include <DirectXMath.h>
#include <iomanip>

namespace features 
{

	AimTracker::AimTracker() 
	{
		this->enabled = true;
	}

	AimTracker::AimTracker(valve::sdk::PlayerEntity *me, valve::sdk::EntityList entity_list) :
		me(me),
		entity_list(entity_list)
	{
		init();
	}

	AimTracker::~AimTracker(){}

	bool AimTracker::is_enabled()
	{
		return this->enabled;
	}

	void AimTracker::toggle()
	{
		this->enabled = !this->enabled;
	}
	void AimTracker::init()
	{
		this->enabled = true;
	}
	void AimTracker::track()
	{	
		DirectX::XMFLOAT3 direction;

		valve::sdk::PlayerEntity *enemy = (this->entity_list + 1)->entity;

		if (!enemy)
		{
			std::cout << "no enemy" << std::endl;
			return;
		}

		if (enemy->is_dormant & 0x100) {
			std::cout << "Out of range ..." << std::endl;
			return;
		}

		DirectX::XMVECTOR enemy_position = DirectX::XMLoadFloat3(reinterpret_cast<DirectX::XMFLOAT3 *>(&enemy->origin));
		DirectX::XMVECTOR my_position = DirectX::XMLoadFloat3(reinterpret_cast<DirectX::XMFLOAT3 *>(&me->origin));

		DirectX::XMStoreFloat3(&direction, DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(enemy_position, my_position)));
		
		// angle between
		float yaw = DirectX::XMConvertToDegrees(std::atan2f(direction.y, direction.x));

		// new pitch
		// Z / hypotenuse, where hypotenuse = 1 cause unit circle
		// But remember to account for 89.0 start Tan 
		// maybe pitch = 89 - arccos(z/hyp)
		float pitch = DirectX::XMConvertToDegrees(std::acosf(direction.z)) - 90.0f;

		//clamp_pitch(pitch); // it's 5 a.m. go to sleep

		std::cout << "Pitch Angle: " << std::dec << std::setprecision(8) << DirectX::XMConvertToDegrees(std::acosf(direction.z)) << std::endl;
		std::cout << "Yaw: " << std::dec << std::setprecision(8) << yaw << std::endl;
		std::cout << "Pitch: " << std::dec << std::setprecision(8) << pitch << std::endl;
		std::cout << "X: " << std::dec << direction.x;
		std::cout << "|Y: " << std::dec << direction.y;
		std::cout << "|Z: " << std::dec << direction.z;
		std::cout << std::endl;
	}

	std::unique_ptr<AimTracker> aimTracker = std::make_unique<AimTracker>();
}
