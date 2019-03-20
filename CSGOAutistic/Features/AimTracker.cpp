#include "AimTracker.h"
#include <iomanip>
#include "../index.h"
#include "../hooks.h"

namespace features 
{
	#define OR ||

	AimTracker::AimTracker()
	{
		this->enabled = true;
		this->no_recoil = false;
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

	DirectX::XMVECTOR AimTracker::target_nearest(std::deque<valve::sdk::PlayerEntity*> candidates)
	{
		DirectX::XMVECTOR my_eyes = DirectX::XMLoadFloat3(reinterpret_cast<DirectX::XMFLOAT3 *>(&(me->origin + me->vec_view_offset)));

		DirectX::XMVECTOR closest_vector;
		valve::sdk::PlayerEntity *nearest_enemy = NULL;
		DirectX::XMVECTOR last_diff;

		for (auto &enemy : candidates)
		{
			if (!nearest_enemy)
			{
				auto enemy_bones = *reinterpret_cast<valve::sdk::bone_matrix *>(enemy->bone_matrix);
				auto head_bone = reinterpret_cast<valve::sdk::Bone *>(&enemy_bones[8]);
				DirectX::XMVECTOR enemy_head = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(head_bone->x, head_bone->y, head_bone->z));

				last_diff = DirectX::XMVectorSubtract(enemy_head, my_eyes);
				closest_vector = DirectX::XMVector3LengthEst(last_diff);
				nearest_enemy = enemy;

			}
			else
			{
				auto enemy_bones = *reinterpret_cast<valve::sdk::bone_matrix *>(enemy->bone_matrix);
				auto head_bone = reinterpret_cast<valve::sdk::Bone *>(&enemy_bones[8]);
				DirectX::XMVECTOR enemy_head = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(head_bone->x, head_bone->y, head_bone->z));

				DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(enemy_head, my_eyes);
				DirectX::XMVECTOR length = DirectX::XMVector3LengthEst(diff);
				if (DirectX::XMVector3LessOrEqual(length, closest_vector))
				{
					closest_vector = length;
					nearest_enemy = enemy;
					last_diff = diff;
				}
			}
		}

		return last_diff;
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

	void AimTracker::adjust_angles(float * angles)
	{
		angles[0] -= me->aim_punch_angle.x * 2.f;
		angles[1] -= me->aim_punch_angle.y * 2.f;
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

	void AimTracker::enable_no_recoil()
	{
		this->no_recoil = true;
	}

	void AimTracker::track(DWORD *user_cmd)
	{	
		DirectX::XMFLOAT3 direction;

		std::deque< valve::sdk::PlayerEntity *> enemy_pool;

		int i = 0;
		for (valve::sdk::EntityList e = entity_list + 1; i < 63; i++, e++)
		{
			valve::sdk::PlayerEntity *p = e->entity;

			if (!p
				OR (p->team_number == me->team_number)
				OR (p->is_dormant & 0x100)
				OR (p->health < 1))
			{
				continue;
			}

			enemy_pool.push_back(p);
		}

		if (enemy_pool.empty())
		{
			return;
		}

		DirectX::XMVECTOR enemy_direction = this->target_nearest(enemy_pool);

		 //HOW TO Call setupbones
		/*matrix3x4a_t MatrixArray[128];
		 entity is localplayer
		if (!pEntity->SetupBones(_THIS, &MatrixArray[0], 128, 0x0100, 0))
			return;*/

		 //SetupBones is currently in IClientRenderable (C_BasePlayer)

		DirectX::XMStoreFloat3(&direction, DirectX::XMVector3Normalize(enemy_direction));
		
		// angle between
		float yaw = DirectX::XMConvertToDegrees(std::atan2f(direction.y, direction.x));

		// new pitch
		// Z / hypotenuse, where hypotenuse = 1 cause unit circle
		// But remember to account for 89.0 start Tan 
		// maybe pitch = 89 - arccos(z/hyp)
		float pitch = DirectX::XMConvertToDegrees(std::acosf(direction.z)) - 90.0f;

		float angles[3] = { pitch, yaw, 0.0f };

		if (this->no_recoil)
		{
			this->adjust_angles(angles);
		}

		clamp_pitch(angles[0]);

		this->set_viewangles(angles);
	}

	std::unique_ptr<AimTracker> aim_tracker = std::make_unique<AimTracker>();
}
