#include "HealthBar.h"
#include <iostream>
#include <algorithm>

namespace features
{

	namespace components
	{

		HealthBar::HealthBar()
		{
		}

		HealthBar::HealthBar(valve::sdk::PlayerEntity *p, std::uint32_t width, valve::sdk::Vector2 position, valve::sdk::Vector3 self_origin) :
			player(p),
			self_origin(self_origin),
			width(width),
			IRenderable(position)
		{

		}

		void HealthBar::render()
		{
			float lerp = lerp_distance();
			utils::render::draw_filled_rect(this->get_position(), this->width, static_cast<std::uint32_t>((this->player->health << 1) * lerp), this->get_color());
			utils::render::draw_outlined_rect(this->get_position(), this->width, static_cast<std::uint32_t>((this->max_health << 1) * lerp), { 0, 0, 0, 255 });
		}

		valve::sdk::Color HealthBar::get_color()
		{
			static const float hue_coefficient = 1.28f; // max_hue / max_health
			auto hue = this->player->health == 0 ? 1.0f : float(this->player->health * hue_coefficient);
			
			return valve::sdk::Color(this->get_rgba(hue));
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

		valve::sdk::Color HealthBar::get_rgba(float const &hue)
		{
			static const float saturation = 0.85f;
			static const float value = 1.00f;

			static const float c = saturation * value;
			static const float m = value - c;

			float x = c * (1 - std::abs((std::fmodf((float(hue) / 60.0f), 2) - 1)));

			float r, g, b;

			switch (std::div(int(hue), 60).quot)
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

			return { this->crv(r, m), this->crv(g, m), this->crv(b, m), static_cast<std::uint32_t>(128.0f * lerp_distance()) };
		}

		constexpr std::uint32_t HealthBar::crv(float const & f, float const & m)
		{
			return std::uint32_t((f + m) * 255.0f);
		}
		float HealthBar::lerp_distance()
		{
			DirectX::XMFLOAT3 distance_vector;

			float const min = 500.0f;
			float const max = 2000.0f;

			DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(
				DirectX::XMLoadFloat3(reinterpret_cast<DirectX::XMFLOAT3 *>(&self_origin)),
				DirectX::XMLoadFloat3(reinterpret_cast<DirectX::XMFLOAT3 *>(&player->origin)));

			DirectX::XMStoreFloat3(&distance_vector, DirectX::XMVector3Length(diff));
			
			float const distance = std::clamp(distance_vector.x, min, max); // x, y, z all contain the same value
			float const lerpVal = 1.0f / (max / ((distance - min) + 1.0f)); // +1.0f to avoid surprise division by zero

			return 1.0f - lerpVal;
		}
	}
}

