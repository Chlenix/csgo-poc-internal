#pragma once

#include "..\Utilities\render.h"
#include <vector>
#include "Components\HealthBar.h"

namespace features
{
	class esp
	{
	public:
		esp();
		~esp();

		void render();

	private:
		std::vector<features::render::components::HealthBar> components;
		void init();
	};

	extern std::unique_ptr<esp> e;

}
