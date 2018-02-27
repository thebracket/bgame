#pragma once

#include "../render_engine/shaders/chunk_shader.hpp"
#include "../render_engine/shaders/voxel_shader.hpp"
#include "../render_engine/shaders/voxel_shadow_shader.hpp"
#include "../render_engine/shaders/lightstage_shader.hpp"
#include "../render_engine/shaders/chunk_depth_shader.hpp"
#include "../render_engine/shaders/ascii_base_shader.hpp"
#include "../render_engine/shaders/ascii_light_shader.hpp"
#include "../render_engine/shaders/worldgen_shader.hpp"
#include <memory>

namespace assets {
    extern unsigned int spriteshader;
    extern std::unique_ptr<worldgen_shader_t> worldgenshader;
    extern std::unique_ptr<chunk_shader_t> chunkshader;
	extern std::unique_ptr<chunk_depth_shader_t> chunkdepthshader;
	extern unsigned int depthquad_shader;
	extern std::unique_ptr<lightstage_shader_t> lightstage_shader;
    extern unsigned int tonemap_shader;
	extern unsigned int sprite_shader;
    extern std::unique_ptr<voxel_shader_t> voxel_shader;
	extern std::unique_ptr<voxel_shadow_shader_t> voxel_shadow_shader;
	extern unsigned int cursor_shader;
	extern unsigned int particle_shader;
	extern unsigned int dirlight_shader;
	extern unsigned int pointlight_shader;
	extern unsigned int lighter_shader;
	extern std::unique_ptr<ascii_base_shader_t> ascii_shader;
	extern std::unique_ptr<ascii_light_shader_t> ascii_light_shader;
}
