#include "AimTracker.h"
#include <DirectXMath.h>
#include <iomanip>
#include "../index.h"
#include "../hooks.h"

namespace features 
{

	AimTracker::AimTracker()
	{
		this->enabled = true;
	}

	AimTracker::AimTracker(valve::sdk::PlayerEntity *me, valve::sdk::EntityList entity_list, utils::vt::VTMananger *vengine) :
		me(me),
		entity_list(entity_list),
		vengine(vengine)
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
	void AimTracker::set_viewangles(float * angles)
	{
		auto fn = hooks::vengine_manager->get_original_vfunc<hooks::fnSetViewAngles>(index::vengine::set_viewangles);
		fn(hooks::vengine_manager->get_class(), angles);
	}

	void AimTracker::clamp_pitch(float & pitch)
	{
		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}

		else if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}
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

		// HOW TO Call setupbones
		/*matrix3x4a_t MatrixArray[128];
		// entity is localplayer
		if (!pEntity->SetupBones(_THIS, &MatrixArray[0], 128, 0x0100, 0))
			return;*/

		// SetupBones is currently in IClientRenderable (C_BasePlayer)

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
		clamp_pitch(pitch); // it's 5 a.m. go to sleep

		std::cout << std::endl;

		float angles[3] = { pitch, yaw, 0.0f };

		this->set_viewangles(angles);
	}

	std::unique_ptr<AimTracker> aim_tracker = std::make_unique<AimTracker>();
}
