#pragma once

#include "../Utilities/VTHook.h"
#include "../sdk.h"

#include <DirectXMath.h>
#include <functional>
#include <deque>

namespace features
{
	class AimTracker
	{

	private:
		bool enabled;
		bool no_recoil;

		valve::sdk::PlayerEntity *me;
		valve::sdk::EntityList entity_list;

	public:
		AimTracker();
		AimTracker(valve::sdk::PlayerEntity *me, valve::sdk::EntityList entity_list);
		~AimTracker();

		bool is_enabled();
		void toggle();

		DirectX::XMVECTOR target_nearest(std::deque<valve::sdk::PlayerEntity*> candidates);

		void track(DWORD *user_cmd);
		void enable_no_recoil();

	private:
		void init();
		void set_viewangles(float *angles);
		void adjust_angles(float *angles);
		void clamp_pitch(float& pitch);

		float const MAX_PITCH = 89.0f;
		float const MIN_PITCH = -89.0f;
	};

	extern std::unique_ptr<AimTracker> aim_tracker;
}

