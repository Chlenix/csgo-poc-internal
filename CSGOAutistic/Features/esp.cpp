#include "..\stdafx.h"
#include "esp.h"
#include "..\Utilities\Scanner.h"
#include <iostream>
#include <iomanip>

namespace features
{

	ESP::ESP()
	{
	}

	ESP::~ESP()
	{
	}
	void ESP::init()
	{
		auto client_panorama = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("client_panorama.dll"));

		DWORD entity_list_offset = utils::scanner::FindPattern("client_panorama.dll", utils::masks::entity_list, utils::patterns::entity_list, 1);
		DWORD view_matrix_offset = 176 + utils::scanner::FindPattern("client_panorama.dll", utils::masks::view_matrix, utils::patterns::view_matrix, 1);

		this->entity_list = reinterpret_cast<valve::sdk::EntityList>(client_panorama + entity_list_offset);
		this->view_matrix = reinterpret_cast<DirectX::XMFLOAT4X4 *>(client_panorama + view_matrix_offset);
	}
	void ESP::render()
	{
		this->scan_entities();

		// TODO: transform position into 2D screen coords

		for (auto &health_bar : this->components)
		{
			health_bar.render();
		}
	}

	void ESP::scan_entities()
	{
		this->components.clear();
		//std::cout << "esp init | matrix[0][0]: " << std::setprecision(3) << std::dec << this->view_matrix->m[0][0] << std::endl;

		valve::sdk::PlayerEntity *me = entity_list->entity;

		int i = 0;
		int j = 0;

		for (valve::sdk::EntityList e = entity_list; e->entity; i++, e++)
		{
			valve::sdk::PlayerEntity *player = e->entity;

			if (player == me)
			{
				this->components.push_back({ player, 10u, { 200u, 250u } });
				//std::cout << "yay" << std::endl;
				continue;
			}

			if (player->is_dormant & 0x100)
			{
				//std::cout << "skipping because dormant 0x" << std::hex << e->entity << std::endl;
				continue;
			}

			if (player->team_number == me->team_number)
			{
				//std::cout << "skipping because same team 0x" << std::hex << e->entity << std::endl;
				continue;
			}

			if (player->health < 1)
			{
				continue;
			}

			j++;

			this->components.push_back({ player, 10u, { 200u + (j * 30u), 250u } });
		}
	}

	valve::sdk::Vector2 ESP::world_to_screen(valve::sdk::Vector3 const & world_position)
	{
		return valve::sdk::Vector2();
	}

	std::unique_ptr<ESP> esp = std::make_unique<ESP>();
}
