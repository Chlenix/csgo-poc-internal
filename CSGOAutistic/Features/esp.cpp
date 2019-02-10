#include "..\stdafx.h"
#include "esp.h"

namespace features
{

	esp::esp()
	{
		init();
	}

	esp::~esp()
	{
	}
	void esp::render()
	{
		for (features::render::components::HealthBar &health_bar : this->components)
		{
			health_bar.render();
		}
	}

	void esp::init()
	{
		features::render::components::HealthBar hb(100, 10, 100, { 300, 250 }, { 112, 255, 112, 255 });
		this->components.push_back(hb);
	}

	std::unique_ptr<esp> e = std::make_unique<esp>();

}
