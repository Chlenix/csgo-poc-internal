#pragma once

#include "IRenderable.h"
#include "..\..\Utilities\render.h"
namespace features
{
	namespace render
	{

		namespace components
		{
			class HealthBar : public IRenderable
			{
			public:
				HealthBar();
				HealthBar(valve::sdk::Vector2 position, valve::sdk::Color color) : 
					IRenderable(position, color) 
				{};
				HealthBar(std::uint32_t hp, std::uint32_t width, std::uint32_t height, valve::sdk::Vector2 position, valve::sdk::Color color) :
					health(hp),
					width(width),
					height(height),
					IRenderable(position, color) 
				{};

				void render() override;


			private:

				std::uint32_t health = 0;
				std::uint32_t height = 0;
				std::uint32_t width = 0;
			};
		}
	}
}

