#pragma once

#include "..\..\sdk.h"
#include <cstdint>
namespace features
{
	namespace render
	{
		namespace components
		{
			class IRenderable
			{
			public:
				IRenderable() = default;
				IRenderable(valve::sdk::Vector2 position, valve::sdk::Color color) :
					position(position),
					color(color)
				{};

				virtual void render() = 0;

#pragma region GETTERS_SETTERS
				valve::sdk::Vector2 get_position()
				{
					return this->position;
				}

				void set_position(valve::sdk::Vector2 v)
				{
					this->position = v;
				}

				valve::sdk::Color get_color()
				{
					return this->color;
				}

				void set_color(valve::sdk::Color c)
				{
					this->color = c;
				}

#pragma endregion
			private:
				valve::sdk::Vector2 position;
				valve::sdk::Color color;
			};
		}
	}
}

