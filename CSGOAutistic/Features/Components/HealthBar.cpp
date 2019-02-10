#include "HealthBar.h"

namespace features
{
	namespace render
	{
		namespace components
		{

			HealthBar::HealthBar()
			{
			}

			void HealthBar::render()
			{
				valve::sdk::Vector2 outline_position = this->get_position();
				utils::render::draw_filled_rect(this->get_position(), this->width, this->height, this->get_color());
				utils::render::draw_outlined_rect(this->get_position(), this->width, this->height, { 0, 0, 0, 255 });
			}
		}
	}
}
