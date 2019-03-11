#pragma once

#include "../sdk.h"

namespace features 
{
	class SpreadController
	{
	private:
		valve::sdk::PlayerEntity &local_player;

		float const predictable_acceleration = 5.0f; // TODO: change to real threshold value

	public:
		SpreadController(valve::sdk::PlayerEntity &local_player);

		void anti_spread();
	};
}