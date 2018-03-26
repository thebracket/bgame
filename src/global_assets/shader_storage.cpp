#include "stdafx.h"
#include "shader_storage.hpp"
#include "../bengine/shaders.hpp"
#include "../render_engine/shaders/chunk_shader.hpp"
#include "../render_engine/shaders/voxel_shader.hpp"
#include "../render_engine/shaders/voxel_shadow_shader.hpp"
#include "../render_engine/shaders/lightstage_shader.hpp"
#include "../render_engine/shaders/chunk_depth_shader.hpp"
#include "../render_engine/shaders/ascii_base_shader.hpp"
#include "../render_engine/shaders/ascii_light_shader.hpp"
#include "../render_engine/shaders/worldgen_shader.hpp"
#include "../render_engine/shaders/cursor_shader.hpp"
#include "../render_engine/shaders/lighter_shader.hpp"
#include "../render_engine/shaders/particle_shader.hpp"
#include "../render_engine/shaders/pointlight_shader.hpp"
#include "../render_engine/shaders/tonemap_shader.hpp"
#include "../render_engine/shaders/skylight_shader.hpp"

namespace assets {
    unsigned int spriteshader = 0;
	std::unique_ptr<worldgen_shader_t> worldgenshader;
	std::unique_ptr<chunk_shader_t> chunkshader;
	std::unique_ptr<chunk_depth_shader_t> chunkdepthshader;
    unsigned int depthquad_shader = 0;
	std::unique_ptr<lightstage_shader_t> lightstage_shader;
    std::unique_ptr<tonemap_shader_t> tonemap_shader;
	std::unique_ptr<voxel_shader_t> voxel_shader;
	std::unique_ptr<voxel_shadow_shader_t> voxel_shadow_shader;
	std::unique_ptr<cursor_shader_t> cursor_shader;
	std::unique_ptr<particle_shader_t> particle_shader;
	std::unique_ptr<pointlight_shader_t> pointlight_shader;
	std::unique_ptr<lighter_shader_t> lighter_shader;
	std::unique_ptr<ascii_base_shader_t> ascii_shader;
	std::unique_ptr<ascii_light_shader_t> ascii_light_shader;
	std::unique_ptr<skylight_shader_t> skylight_shader;
	unsigned int skyshader = 0;
	unsigned int fluid_shader = 0;

	void initialize_shaders()
	{
		using namespace bengine;

		spriteshader = load_shaders("game_assets/shaders45/spriteshader_vertex.glsl", "game_assets/shaders45/spriteshader_fragment.glsl");
		worldgenshader = std::make_unique<worldgen_shader_t>();
		chunkshader = std::make_unique<chunk_shader_t>();
		chunkdepthshader = std::make_unique<chunk_depth_shader_t>();
		depthquad_shader = load_shaders("game_assets/shaders45/depthquad_vertex.glsl", "game_assets/shaders45/depthquad_fragment.glsl");
		lightstage_shader = std::make_unique<lightstage_shader_t>();
		tonemap_shader = std::make_unique<tonemap_shader_t>();
		voxel_shader = std::make_unique<voxel_shader_t>();
		voxel_shadow_shader = std::make_unique<voxel_shadow_shader_t>();
		cursor_shader = std::make_unique<cursor_shader_t>();
		particle_shader = std::make_unique<particle_shader_t>();
		lighter_shader = std::make_unique<lighter_shader_t>();
		pointlight_shader = std::make_unique<pointlight_shader_t>();
		ascii_shader = std::make_unique<ascii_base_shader_t>();
		ascii_light_shader = std::make_unique<ascii_light_shader_t>();
		skyshader = load_shaders("game_assets/shaders45/skyshader_vertex.glsl", "game_assets/shaders45/skyshader_fragment.glsl");
		skylight_shader = std::make_unique<skylight_shader_t>();
		fluid_shader = load_shaders("game_assets/shaders45/compute/fluid_flow.glsl");
	}
}