#include "shader_storage.hpp"

namespace assets {
    unsigned int spriteshader = 0;
    unsigned int worldgenshader = 0;
	std::unique_ptr<chunk_shader_t> chunkshader;
    unsigned int depthquad_shader = 0;
    unsigned int lightstage_shader = 0;
    unsigned int tonemap_shader = 0;
    unsigned int bloom_shader = 0;
	unsigned int sprite_shader = 0;
	std::unique_ptr<voxel_shader_t> voxel_shader;
}