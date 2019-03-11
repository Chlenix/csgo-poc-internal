#pragma once

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
	public:
		AimTracker();
		AimTracker(valve::sdk::PlayerEntity *me, valve::sdk::EntityList entity_list);
		~AimTracker();

		bool is_enabled();
		void toggle();

		void track();

	private:
		void init();

		float const MAX_PITCH = 89.000f;
		float const MIN_PITCH = -89.000f;
	};

	extern std::unique_ptr<AimTracker> aimTracker;
}

