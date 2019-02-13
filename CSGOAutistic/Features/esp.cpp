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
		this->renderable.clear();
		this->scan_entities();

		// TODO: transform position into 2D screen coords

		for (auto &player : this->renderable)
		{
			valve::sdk::Vector2 screen_position;
			if (this->world_to_screen(player->origin, screen_position))
			{
				components::HealthBar{ player, 10u, screen_position }.render();
			}
		}
	}

	bool ESP::world_to_screen(valve::sdk::Vector3 const &position, valve::sdk::Vector2 &out_screen)
	{
		// shamelessly borrowed from GuidedHacking.com
		float x_value = position.x * this->view_matrix->m[0][0] + position.y * this->view_matrix->m[0][1] + position.z * this->view_matrix->m[0][2] + this->view_matrix->m[0][3];
		float y_value = position.x * this->view_matrix->m[1][0] + position.y * this->view_matrix->m[1][1] + position.z * this->view_matrix->m[1][2] + this->view_matrix->m[1][3];

		float w_value = position.x * this->view_matrix->m[3][0] + position.y * this->view_matrix->m[3][1] + position.z * this->view_matrix->m[3][2] + this->view_matrix->m[3][3];

		if (w_value < 1.0f)
		{
			// maybe fiddle with the value for your specific game, but this should work for most id imagine
			return false;
		}

		x_value /= w_value;
		y_value /= w_value;

		// Next, you half those values and add 0.5.
		x_value /= 2;
		y_value /= 2;
		// Add that 0.5.  Why the fuck 0.5? I was curious too. Apparently, in device space, pixel centers lie at 0.5 offsets.
		// So technically ( 0, 0 ) is ( 0.5, 0.5 ). Shits funky. Maybe ive misinterpreted it but thats what it appears to be.
		x_value += .5f;
		y_value += .5f;
		// Lastly, you multiply by your screen dimensions, how else are you gonna scale it!?!?
		// Dont be a square. Get your screen dimensions programmatically. If you're on dx9, pDevice->GetViewPort() and the view port will spit out width+height.
		// DX11 is similar except you get it from the swapchain.
		x_value *= 1600;
		y_value *= 900;

		// clamp
		if (x_value > 1600 || x_value < 0)
		{
			return false;
		}

		if (y_value > 900 || y_value < 0)
		{
			return false;
		}

		y_value = 900 - y_value;
		y_value -= 100;

		if (y_value < 0)
		{
			return false;
		}

		x_value += 30;

		if (x_value > 1600)
		{
			return false;
		}

		out_screen = { static_cast<std::uint32_t>(x_value), static_cast<std::uint32_t>(y_value) };
		return true;
	}

	void ESP::scan_entities()
	{
		//std::cout << "esp init | matrix[0][0]: " << std::setprecision(3) << std::dec << this->view_matrix->m[0][0] << std::endl;

		valve::sdk::PlayerEntity *me = entity_list->entity;

		int i = 0;

		for (valve::sdk::EntityList e = entity_list; e->entity; i++, e++)
		{
			valve::sdk::PlayerEntity *player = e->entity;

			if (player == me)
			{
				this->renderable.push_back(player);
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

			this->renderable.push_back(player);
		}
	}

	std::unique_ptr<ESP> esp = std::make_unique<ESP>();
}
