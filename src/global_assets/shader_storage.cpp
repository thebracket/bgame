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
	unsigned int cursor_shader;
	unsigned int particle_shader;
	unsigned int dirlight_shader;
	unsigned int pointlight_shader;
	unsigned int lighter_shader;
	std::unique_ptr<ascii_base_shader_t> ascii_shader;
	std::unique_ptr<ascii_light_shader_t> ascii_light_shader;

	void initialize_shaders()
	{
		using namespace bengine;

		spriteshader = load_shaders("game_assets/spriteshader_vertex.glsl", "game_assets/spriteshader_fragment.glsl");
		worldgenshader = std::make_unique<worldgen_shader_t>();
		chunkshader = std::make_unique<chunk_shader_t>();
		chunkdepthshader = std::make_unique<chunk_depth_shader_t>();
		depthquad_shader = load_shaders("game_assets/depthquad_vertex.glsl", "game_assets/depthquad_fragment.glsl");
		lightstage_shader = std::make_unique<lightstage_shader_t>();
		tonemap_shader = load_shaders("game_assets/tonemap_vertex.glsl", "game_assets/tonemap_fragment.glsl");
		sprite_shader = load_shaders("game_assets/sprite_shader_vertex.glsl", "game_assets/sprite_shader_fragment.glsl");
		voxel_shader = std::make_unique<voxel_shader_t>();
		voxel_shadow_shader = std::make_unique<voxel_shadow_shader_t>();
		cursor_shader = load_shaders("game_assets/highlight_vertex.glsl", "game_assets/highlight_fragment.glsl");
		particle_shader = load_shaders("game_assets/particle_vertex.glsl", "game_assets/particle_fragment.glsl");
		dirlight_shader = load_shaders("game_assets/dirlight_vertex.glsl", "game_assets/dirlight_fragment.glsl");
		lighter_shader = load_shaders("game_assets/lighter_vertex.glsl", "game_assets/lighter_fragment.glsl");
		pointlight_shader = load_shaders("game_assets/pointlight_vertex.glsl", "game_assets/pointlight_fragment.glsl", "game_assets/pointlight_geometry.glsl");
		ascii_shader = std::make_unique<ascii_base_shader_t>();
		ascii_light_shader = std::make_unique<ascii_light_shader_t>();
	}
}