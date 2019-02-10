#pragma once

#include <cstdint>

namespace valve
{
	namespace sdk
	{
		typedef struct _Color
		{
			std::uint32_t r;
			std::uint32_t g;
			std::uint32_t b;
			std::uint32_t a;
		} Color;

		typedef struct _ScreenPosition
		{
			std::uint32_t x;
			std::uint32_t y;
		} Vector2;
	}
}