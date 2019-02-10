#pragma once

#include "..\sdk.h"
#include "..\index.h"
#include "..\hooks.h"
//#include "VTHook.h"

namespace utils 
{
	namespace render
	{
		inline void set_color(valve::sdk::Color const &color)
		{
			hooks::fnSetRGBA f = hooks::surface_manager->get_original_vfunc<hooks::fnSetRGBA>(index::surface::set_color);
			f(hooks::surface_manager->get_class(), color.r, color.g, color.b, color.a);
		}

		inline void draw_filled_rect(valve::sdk::Vector2 const &position, int const &width, int const &height, valve::sdk::Color const &color)
		{
			set_color(color);
			hooks::fnDrawFilledRect f = hooks::surface_manager->get_original_vfunc<hooks::fnDrawFilledRect>(index::surface::draw_filled_rect);
			f(hooks::surface_manager->get_class(), position.x, position.y, position.x + width, position.y + height);
		}

		inline void draw_outlined_rect(valve::sdk::Vector2 const &position, int const &width, int const &height, valve::sdk::Color const &color)
		{
			set_color(color);
			hooks::fnDrawOutlinedRect f = hooks::surface_manager->get_original_vfunc<hooks::fnDrawOutlinedRect>(index::surface::draw_outlined_rect);
			f(hooks::surface_manager->get_class(), position.x, position.y, position.x + width, position.y + height);
		}
	}
}

