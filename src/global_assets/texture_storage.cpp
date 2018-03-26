#include "stdafx.h"
#include "texture_storage.hpp"
#include "../bengine/textures.hpp"

namespace assets {
    std::unique_ptr<bengine::texture_t> bracket_logo;
    std::unique_ptr<bengine::texture_t> background_image;
	std::unique_ptr<bengine::texture_t> bracket;
    std::unique_ptr<bengine::texture_t> game_logo;
    std::unique_ptr<bengine::texture_t> starfield;
    unsigned int worldgen_texture_array;
    unsigned int chunk_texture_array;
	unsigned int cursor_texture_array;
	std::unique_ptr<bengine::texture_t> ascii_texture;
}