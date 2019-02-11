#pragma once

#include "IRenderable.h"
#include "..\..\Utilities\render.h"
namespace features
{
	namespace components
	{
		class HealthBar : public IRenderable
		{
		public:
			HealthBar();
			HealthBar(valve::sdk::PlayerEntity *p, std::uint32_t width, valve::sdk::Vector2 position);
			HealthBar(std::int32_t hp, std::uint32_t width, std::uint32_t height, valve::sdk::Vector2 position) :
				health(hp),
				width(width),
				height(height),
				IRenderable(position)
			{};

			void render() override;

			valve::sdk::Color get_color();
			void set_health(std::int32_t health);
			void add_health(std::int32_t health_modifier);

		private:
			valve::sdk::Color get_rgb(float const &alpha);
			std::uint32_t constexpr crv(float const& f, float const& m);

		private:

			valve::sdk::PlayerEntity *player;

			std::int32_t health = 0;

			std::int32_t const min_health = 0;
			std::int32_t const max_health = 100;

			std::uint32_t width = 0;
			std::uint32_t height = 0;
		};
	}
}


