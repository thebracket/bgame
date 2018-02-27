#pragma once

#include <memory>

// Forward declarations
class worldgen_shader_t;
class chunk_shader_t;
class chunk_depth_shader_t;
class lightstage_shader_t;
class voxel_shader_t;
class voxel_shadow_shader_t;
class ascii_base_shader_t;
class ascii_light_shader_t;
class cursor_shader_t;
class lighter_shader_t;
class particle_shader_t;
class pointlight_shader_t;
class tonemap_shader_t;

namespace assets {
    extern unsigned int spriteshader;
    extern std::unique_ptr<worldgen_shader_t> worldgenshader;
    extern std::unique_ptr<chunk_shader_t> chunkshader;
	extern std::unique_ptr<chunk_depth_shader_t> chunkdepthshader;
	extern unsigned int depthquad_shader;
	extern std::unique_ptr<lightstage_shader_t> lightstage_shader;
    extern std::unique_ptr<tonemap_shader_t> tonemap_shader;
    extern std::unique_ptr<voxel_shader_t> voxel_shader;
	extern std::unique_ptr<voxel_shadow_shader_t> voxel_shadow_shader;
	extern std::unique_ptr<cursor_shader_t> cursor_shader;
	extern std::unique_ptr<particle_shader_t> particle_shader;
	extern std::unique_ptr<pointlight_shader_t> pointlight_shader;
	extern std::unique_ptr<lighter_shader_t> lighter_shader;
	extern std::unique_ptr<ascii_base_shader_t> ascii_shader;
	extern std::unique_ptr<ascii_light_shader_t> ascii_light_shader;

	void initialize_shaders();
}
