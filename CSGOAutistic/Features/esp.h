#pragma once

#include "..\Utilities\render.h"
#include <vector>
#include "Components\HealthBar.h"
#include "..\sdk.h"
#include <DirectXMath.h>

namespace features
{
	class ESP
	{
	public:
		ESP();
		~ESP();

		void init();
		void scan_entities();
		void render();

	private:
		bool world_to_screen(valve::sdk::Vector3 const &world_position, valve::sdk::Vector2 &out_screen);

	private:

		valve::sdk::EntityList entity_list;
		DirectX::XMFLOAT4X4 *view_matrix;

		std::vector<valve::sdk::PlayerEntity *> renderable;
		int iters = 0;
	};

	extern std::unique_ptr<ESP> esp;
}
