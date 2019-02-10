#include "..\stdafx.h"
#include "esp.h"

namespace features
{

	ESP::ESP()
	{
		init();
	}

	ESP::~ESP()
	{
	}
	void ESP::render()
	{
		for (auto &health_bar : this->components)
		{
			health_bar.render();
		}
	}

	void ESP::init()
	{
		int hp = 100;
		features::components::HealthBar hb(hp, 10, hp << 1, { 300, 250 });
		this->components.push_back(hb);
	}

	std::unique_ptr<ESP> esp = std::make_unique<ESP>();

}
