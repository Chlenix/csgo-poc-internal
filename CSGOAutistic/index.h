#pragma once
#include <cstdint>
namespace index
{
	namespace vgui
	{
		constexpr ::std::uint32_t paint_traverse = 41;
		constexpr ::std::uint32_t get_name = 36;
	}

	namespace vengine
	{
		constexpr ::std::uint32_t get_viewangles = 18;
		constexpr ::std::uint32_t set_viewangles = 19;
	}

	namespace surface
	{
		constexpr ::std::uint32_t set_color = 15;
		constexpr ::std::uint32_t draw_filled_rect = 16;
		constexpr ::std::uint32_t draw_outlined_rect = 18;
		constexpr ::std::uint32_t set_text_color = 25;
		constexpr ::std::uint32_t set_text_position = 26;
		constexpr ::std::uint32_t draw_print_text = 28;
	}
}