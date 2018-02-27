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

namespace assets {
    unsigned int spriteshader = 0;
	std::unique_ptr<worldgen_shader_t> worldgenshader;
	std::unique_ptr<chunk_shader_t> chunkshader;
	std::unique_ptr<chunk_depth_shader_t> chunkdepthshader;
    unsigned int depthquad_shader = 0;
	std::unique_ptr<lightstage_shader_t> lightstage_shader;
    unsigned int tonemap_shader = 0;
	unsigned int sprite_shader = 0;
	std::unique_ptr<voxel_shader_t> voxel_shader;
	std::unique_ptr<voxel_shadow_shader_t> voxel_shadow_shader;
	std::unique_ptr<cursor_shader_t> cursor_shader;
	unsigned int particle_shader;
	unsigned int pointlight_shader;
	std::unique_ptr<lighter_shader_t> lighter_shader;
	std::unique_ptr<ascii_base_shader_t> ascii_shader;
	std::unique_ptr<ascii_light_shader_t> ascii_light_shader;

	void initialize_shaders()
	{
		using namespace bengine;

		spriteshader = load_shaders("game_assets/shaders45/spriteshader_vertex.glsl", "game_assets/shaders45/spriteshader_fragment.glsl");
		worldgenshader = std::make_unique<worldgen_shader_t>();
		chunkshader = std::make_unique<chunk_shader_t>();
		chunkdepthshader = std::make_unique<chunk_depth_shader_t>();
		depthquad_shader = load_shaders("game_assets/shaders45/depthquad_vertex.glsl", "game_assets/shaders45/depthquad_fragment.glsl");
		lightstage_shader = std::make_unique<lightstage_shader_t>();
		tonemap_shader = load_shaders("game_assets/tonemap_vertex.glsl", "game_assets/tonemap_fragment.glsl");
		sprite_shader = load_shaders("game_assets/sprite_shader_vertex.glsl", "game_assets/sprite_shader_fragment.glsl");
		voxel_shader = std::make_unique<voxel_shader_t>();
		voxel_shadow_shader = std::make_unique<voxel_shadow_shader_t>();
		cursor_shader = std::make_unique<cursor_shader_t>();
		particle_shader = load_shaders("game_assets/particle_vertex.glsl", "game_assets/particle_fragment.glsl");
		lighter_shader = std::make_unique<lighter_shader_t>();
		pointlight_shader = load_shaders("game_assets/pointlight_vertex.glsl", "game_assets/pointlight_fragment.glsl", "game_assets/pointlight_geometry.glsl");
		ascii_shader = std::make_unique<ascii_base_shader_t>();
		ascii_light_shader = std::make_unique<ascii_light_shader_t>();
	}
}