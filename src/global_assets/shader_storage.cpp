#include "shader_storage.hpp"

namespace assets {
    unsigned int spriteshader = 0;
    unsigned int worldgenshader = 0;
	std::unique_ptr<chunk_shader_t> chunkshader;
	std::unique_ptr<chunk_depth_shader_t> chunkdepthshader;
    unsigned int depthquad_shader = 0;
	std::unique_ptr<lightstage_shader_t> lightstage_shader;
    unsigned int tonemap_shader = 0;
	unsigned int sprite_shader = 0;
	std::unique_ptr<voxel_shader_t> voxel_shader;
	std::unique_ptr<voxel_shadow_shader_t> voxel_shadow_shader;
	unsigned int cursor_shader;
	unsigned int particle_shader;
	unsigned int dirlight_shader;
	unsigned int pointlight_shader;
	unsigned int lighter_shader;
	std::unique_ptr<ascii_base_shader_t> ascii_shader;
	std::unique_ptr<ascii_light_shader_t> ascii_light_shader;
}