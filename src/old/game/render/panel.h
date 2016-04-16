#pragma once

#include <string>
#include "../../engine/sdl2_backend.hpp"

using std::string;
using engine::sdl2_backend;

namespace render
{

class panel
{
public:
	panel(sdl2_backend * SDL, const int &w, const int &h) :
			sdl(SDL), width(w), height(h)
	{
		texture_id = sdl->create_temporary_texture(w, h);
	}

	~panel()
	{
		sdl->mark_texture_for_deletion(texture_id);
	}

	sdl2_backend * sdl;
	string texture_id;
	const int width;
	const int height;
};

}
