#include "SpreadController.h"

namespace features
{
	SpreadController::SpreadController(valve::sdk::PlayerEntity & local_player) :
		local_player(local_player)
	{}

	void SpreadController::anti_spread()
	{
		this->local_player.aim_punch_angle;
	};
}
