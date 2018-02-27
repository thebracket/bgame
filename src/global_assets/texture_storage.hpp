#pragma once
#include <memory>

// Forward declarations
namespace bengine {
    struct texture_t;
}

namespace assets {
    extern std::unique_ptr<bengine::texture_t> bracket_logo;
    extern std::unique_ptr<bengine::texture_t> background_image;
	extern std::unique_ptr<bengine::texture_t> bracket;
    extern std::unique_ptr<bengine::texture_t> game_logo;
    extern std::unique_ptr<bengine::texture_t> starfield;
    extern unsigned int worldgen_texture_array;
    extern unsigned int chunk_texture_array;
	extern unsigned int cursor_texture_array;
	extern std::unique_ptr<bengine::texture_t> ascii_texture;
}