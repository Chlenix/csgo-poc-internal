#pragma once

#include "../Utilities/VTHook.h"
#include "../sdk.h"

#include <functional>

namespace features
{
	class AimTracker
	{

	private:
		bool enabled;

		valve::sdk::PlayerEntity *me;
		valve::sdk::EntityList entity_list;
		utils::vt::VTMananger *vengine;

	public:
		AimTracker();
		AimTracker(valve::sdk::PlayerEntity *me, valve::sdk::EntityList entity_list, utils::vt::VTMananger *vengine);
		~AimTracker();

		bool is_enabled();
		void toggle();

		void track();

	private:
		void init();
		void set_viewangles(float *angles);
		void clamp_pitch(float& pitch);

		float const MAX_PITCH = 89.0f;
		float const MIN_PITCH = -89.0f;
	};

	extern std::unique_ptr<AimTracker> aim_tracker;
}

