#include "HealthBar.h"
#include <iostream>

namespace features
{

	namespace components
	{

		HealthBar::HealthBar()
		{
		}

		void HealthBar::render()
		{
			utils::render::draw_filled_rect(this->get_position(), this->width, this->health << 1, this->get_color());
			utils::render::draw_outlined_rect(this->get_position(), this->width, this->max_health << 1, { 0, 0, 0, 255 });
		}


		valve::sdk::Color HealthBar::get_color()
		{
			static const float hue_coefficient = 1.28f; // max_hue / max_health

			this->hue = this->health == 0 ? 1.0f : this->health * hue_coefficient;
			return this->get_rgb(255u);
		}

		void HealthBar::set_health(std::int32_t health)
		{
			this->health = health;
		}
		void HealthBar::add_health(std::int32_t health_modifier)
		{
			auto new_health = this->health + health_modifier;
			if (new_health < min_health)
			{
				new_health = min_health;
			}

			if (new_health > max_health)
			{
				new_health = max_health;
			}

			this->health = new_health;
			std::cout << std::dec << this->health << std::endl;
		}

		valve::sdk::Color HealthBar::get_rgb(std::uint32_t const &alpha)
		{
			static const float saturation = 0.85f; // TODO: make it depend on the distance to the target 
			static const float value = 1.00f;

			static const float c = saturation * value;
			static const float m = value - c;

			float x = c * (1 - std::abs((std::fmodf((float(this->hue) / 60.0f), 2) - 1)));

			float r, g, b;

			switch (std::div(this->hue, 60).quot)
			{
			case 0:
				r = c; g = x; b = 0;
				break;
			case 1:
				r = x; g = c; b = 0;
				break;
			case 2:
				r = 0; g = c; b = x;
				break;
			case 3:
				r = 0; g = x; b = c;
				break;
			case 4:
				r = x; g = 0; b = c;
				break;
			case 5:
				r = c; g = 0; b = x;
				break;
			default:
				return { 0, 0, 0, 255 };
			}

			return { this->crv(r, m), this->crv(g, m), this->crv(b, m), alpha };
		}

		constexpr std::uint32_t HealthBar::crv(float const & f, float const & m)
		{
			return std::uint32_t((f + m) * 255.0f);
		}
	}
}

