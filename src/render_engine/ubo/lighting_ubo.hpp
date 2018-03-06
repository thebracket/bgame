#pragma once
#include "../pointlights.hpp"
#include <map>

namespace render
{
	namespace lighting_ubo
	{
		extern unsigned int ubo;
		void update_lights(std::map<std::size_t, pointlight_t> &pointlights);
	}
}
