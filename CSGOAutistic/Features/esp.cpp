#include "..\stdafx.h"
#include "esp.h"
#include "..\Utilities\Scanner.h"
#include <iostream>
#include "..\sdk.h"

namespace features
{

	ESP::ESP()
	{
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
		auto client_panorama = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("client_panorama.dll"));
		auto entity_list = reinterpret_cast<valve::sdk::EntityList>(client_panorama + utils::scanner::FindPattern("client_panorama.dll", "x????xxxxx?x", "\xBB\x11\x11\x11\x11\x90\x83\xFF\x01\x7C\x11\xA1", 1));

		std::cout << "esp init 0x" << std::hex << entity_list << std::endl;

		valve::sdk::PlayerEntity *me = entity_list->entity;

		int i = 0;
		int j = 0;
		for (valve::sdk::EntityList e = entity_list; e->entity; i++, e++)
		{
			valve::sdk::PlayerEntity *player = e->entity;

			if (player == me)
			{
				this->components.push_back({ player, 10u, { 200u, 250u } });
				std::cout << "yay" << std::endl;
				continue;
			}

			if (player->is_dormant & 0x100)
			{
				std::cout << "skipping because dormant 0x" << std::hex << e->entity << std::endl;
				continue;
			}

			if (player->team_number == me->team_number)
			{
				std::cout << "skipping because same team 0x" << std::hex << e->entity << std::endl;
				continue;
			}

			j++;

			this->components.push_back({ player, 10u, { 200u + (j * 30u), 250u } }); // TODO: maybe change to 250u + modifier?
		}
	}

	std::unique_ptr<ESP> esp = std::make_unique<ESP>();
}
