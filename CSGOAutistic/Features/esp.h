#pragma once

#include "..\Utilities\render.h"
#include <vector>
#include "Components\HealthBar.h"
#include <type_traits>

namespace features
{
	class ESP
	{
	public:
		ESP();
		~ESP();

		void render();

		void increase_health(std::uint32_t health_modifier)
		{
			for (auto &v : this->components)
			{
				v.add_health(health_modifier);
			}
		}

	private:
		std::vector<features::components::HealthBar> components;
		void init();
	};

	extern std::unique_ptr<ESP> esp;
}
