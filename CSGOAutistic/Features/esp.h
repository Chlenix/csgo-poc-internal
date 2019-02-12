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

		void increase_health(std::uint32_t health_modifier)
		{
			for (auto &v : this->components)
			{
				v.add_health(health_modifier);
			}
		}

	private:
		valve::sdk::Vector2 world_to_screen(valve::sdk::Vector3 const &world_position);

	private:

		valve::sdk::EntityList entity_list;
		DirectX::XMFLOAT4X4 *view_matrix;

		std::vector<features::components::HealthBar> components;
		int iters = 0;
	};

	extern std::unique_ptr<ESP> esp;
}
