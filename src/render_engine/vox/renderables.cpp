#include "renderables.hpp"
#include "../../global_assets/game_camera.hpp"
#include "voxel_model.hpp"
#include "voxreader.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../global_assets/game_building.hpp"
#include "../../raws/buildings.hpp"
#include "../../raws/defs/building_def_t.hpp"
#include "../../raws/species.hpp"
#include "../../raws/defs/civilization_t.hpp"
#include "../../systems/mouse.hpp"
#include "../../planet/region/region.hpp"
#include "../../systems/ai/inventory_system.hpp"
#include "../chunks/chunks.hpp"
#include "../renderbuffers.hpp"
#include "../camera.hpp"
#include "../../global_assets/game_ecs.hpp"

using namespace tile_flags;

namespace render {

	bool models_changed = true;
	static std::unique_ptr<boost::container::flat_map<int, std::vector<vox::instance_t>>> models_to_render;
	std::vector<std::unique_ptr<vox::voxel_render_buffer_t>> model_buffers;
	static unsigned int sprite_vao = 0;
	static unsigned int sprite_vbo = 0;
	static unsigned int glyph_vao = 0;
	static unsigned int glyph_vbo = 0;

	static inline void add_voxel_model(const int &model, const float &x, const float &y, const float &z, const float &red, const float &green, const float &blue, const float angle=0.0f, const float x_rot=0.0f, const float y_rot=0.0f, const float z_rot=0.0f) {
		auto finder = models_to_render->find(model);
		if (finder != models_to_render->end()) {
			finder->second.push_back(vox::instance_t{x, y, z, x_rot, y_rot, z_rot, angle, red, green, blue});
		}
		else {
			models_to_render->insert(std::make_pair(model, std::vector<vox::instance_t>{vox::instance_t{x, y, z, x_rot, y_rot, z_rot, angle, red, green, blue}}));
		}
	}

	static void build_voxel_buildings() {
		bengine::each<building_t, position_t>(
			[](bengine::entity_t &e, building_t &b, position_t &pos) {
			if (pos.z > camera_position->region_z - 10 && pos.z <= camera_position->region_z) {
				const int chunkidx = chunks::chunk_id_by_world_pos(pos.x, pos.y, pos.z);
				if (visible_chunk_set.find(chunkidx) != visible_chunk_set.end()) {
					if (b.vox_model > 0) {
						//std::cout << "Found model #" << b.vox_model << "\n";
						auto x = static_cast<float>(pos.x);
						const auto y = static_cast<float>(pos.z);
						auto z = static_cast<float>(pos.y);

						//std::cout << b.width << " x " << b.height << "\n";

						auto red = 1.0f;
						auto green = 1.0f;
						auto blue = 1.0f;

						if (!b.complete) {
							red = 0.1f;
							green = 0.1f;
							blue = 0.1f;
						}

						add_voxel_model(b.vox_model, x, y, z, red, green, blue, static_cast<float>(pos.rotation) * 3.14159265358979323846f, 0.0f, 1.0f, 0.0f);
					}
				}
			}
		});

		if (game_master_mode == DESIGN && game_design_mode == BUILDING && buildings::has_build_mode_building) {
			// We have a building selected; determine if it can be built and show it
			const auto tag = buildings::build_mode_building.tag;
			const auto building_def = get_building_def(tag);
			if (building_def && building_def->vox_model > 0) {
				// We have the model and the definition; see if its possible to build
				auto can_build = true;

				const auto bx = systems::mouse_wx + (building_def->width == 3 ? -1 : 0);
				const auto by = systems::mouse_wy + (building_def->height == 3 ? -1 : 0);
				const auto bz = systems::mouse_wz;

				std::vector<int> target_tiles;
				for (auto y = by; y < by + building_def->height; ++y) {
					for (auto x = bx; x < bx + building_def->width; ++x) {
						const auto idx = mapidx(x, y, bz);
						if (!region::flag(idx, CAN_STAND_HERE))
						{
							// TODO: Check or open space and allow that for some tags.
							if (!(tag == "floor" || tag == "wall"))
							{
								can_build = false;
							}
						}
						if (region::get_building_id(idx) > 0) can_build = false;
						target_tiles.emplace_back(idx);
					}
				}

				if (can_build) {

					add_voxel_model(building_def->vox_model, static_cast<float>(bx), static_cast<float>(bz), static_cast<float>(by), 1.0f, 1.0f, 1.0f);

					if (systems::left_click) {
						// Perform the building
						systems::inventory_system::building_request(bx, by, bz, buildings::build_mode_building);
						buildings::has_build_mode_building = false;
					}
				}
				else {
					add_voxel_model(building_def->vox_model, static_cast<float>(bx), static_cast<float>(bz), static_cast<float>(by), 1.0f, 0.0f, 0.0f);
				}
			}
		}
	}

	static void build_voxel_items() {
		bengine::each<renderable_t, position_t>([](bengine::entity_t &e, renderable_t &r, position_t &pos) {
			if (pos.z > camera_position->region_z - 10 && pos.z <= camera_position->region_z) {
				const int chunkidx = chunks::chunk_id_by_world_pos(pos.x, pos.y, pos.z);
				if (visible_chunk_set.find(chunkidx) != visible_chunk_set.end()) {
					if (r.vox > 0) {
						auto x = static_cast<float>(pos.x);
						const auto y = static_cast<float>(pos.z);
						auto z = static_cast<float>(pos.y);

						add_voxel_model(r.vox, x, y, z, 1.0f, 1.0f, 1.0f);
					}
				}
			}
		});
	}

	static bool is_lying_down(bengine::entity_t &e)
	{
		const auto sleepy = e.component<sleep_clock_t>();
		if (sleepy && sleepy->is_sleeping) return true;
		const auto health = e.component<health_t>();
		return (health && health->unconscious);
	}
	
	struct composite_cache_t
	{
		int voxel_model;
		float r, g, b;
	};

	static std::map<int, std::vector<composite_cache_t>> composite_cache;

	void invalidate_composite_cache_for_entity(const int &id)
	{
		composite_cache.erase(id);
	}

	static void render_settler(bengine::entity_t &e, renderable_composite_t &r, position_t &pos) {
		const auto species = e.component<species_t>();
		if (!species) return;

		if (pos.z > camera_position->region_z - 10 && pos.z <= camera_position->region_z) {
			const auto chunkidx = chunks::chunk_id_by_world_pos(pos.x, pos.y, pos.z);
			if (visible_chunk_set.find(chunkidx) != visible_chunk_set.end()) {
				const auto inner_x = static_cast<float>(pos.x);
				const auto inner_y = static_cast<float>(pos.z);
				const auto inner_z = static_cast<float>(pos.y);

				const auto is_upright = !is_lying_down(e);

				const auto rotation = is_upright ? (static_cast<float>(pos.rotation) * 3.14159265358979323846f) / 180.0f : 1.5707963267948966192313216916398f;
				const auto rot1 = is_upright ? 0.0f : 1.0f;
				const auto rot2 = is_upright ? 1.0f : 0.0f;
				const auto rot3 = 0.0f;

				const auto cache_finder = composite_cache.find(e.id);
				if (cache_finder != composite_cache.end())
				{
					for (const auto &c : cache_finder->second)
					{
						add_voxel_model(c.voxel_model, inner_x, inner_y, inner_z, c.r, c.g, c.b, rotation, rot1, rot2, rot3);
					}
					return;
				}
				std::vector<composite_cache_t> cc;

				// Clip check passed - add the model
				add_voxel_model(49, inner_x, inner_y, inner_z, species->skin_color.second.r, species->skin_color.second.g, species->skin_color.second.b, rotation, rot1, rot2, rot3);
				cc.emplace_back(composite_cache_t{ 49, species->skin_color.second.r, species->skin_color.second.g, species->skin_color.second.b });
				
				// Add hair
				int hair_vox;
				switch (species->hair_style) {
				case SHORT_HAIR: hair_vox = 50; break;
				case LONG_HAIR: hair_vox = 51; break;
				case PIGTAILS: hair_vox = 52; break;
				case MOHAWK: hair_vox = 53; break;
				case BALDING: hair_vox = 54; break;
				case TRIANGLE: hair_vox = 55; break;
				default: hair_vox = 0;
				}
				if (hair_vox > 0) {
					add_voxel_model(hair_vox, inner_x, inner_y, inner_z, species->hair_color.second.r, species->hair_color.second.g, species->hair_color.second.b, rotation, rot1, rot2, rot3);
					cc.emplace_back(composite_cache_t{ hair_vox, species->hair_color.second.r, species->hair_color.second.g, species->hair_color.second.b });
				}				

				// Add items
				using namespace bengine;
				each<item_t, item_carried_t>([&e, &inner_x, &inner_y, &inner_z, &rotation, &rot1, &rot2, &rot3, &cc](entity_t &E, item_t &item, item_carried_t &carried) {
					if (carried.carried_by == e.id && item.clothing_layer > 0) {
						add_voxel_model(item.clothing_layer, inner_x, inner_y, inner_z, item.clothing_color.r, item.clothing_color.g, item.clothing_color.b, rotation, rot1, rot2, rot3);
						cc.emplace_back(composite_cache_t{ item.clothing_layer, item.clothing_color.r, item.clothing_color.g, item.clothing_color.b });
					}
				});

				composite_cache.insert(std::make_pair(e.id, cc));
			}
		}
	}

	static void render_composite_sentient(bengine::entity_t &e, renderable_composite_t &r, position_t &pos) {
		// TODO: This should follow a different code path
		//std::cout << "Render sentient\n";
		const auto idx = mapidx(pos);
		if (region::flag(idx, VISIBLE)) {
			render_settler(e, r, pos);
		}
	}

	static void build_composites() {
		bengine::each<renderable_composite_t, position_t>([](bengine::entity_t &e, renderable_composite_t &r, position_t &pos) {
			//std::cout << r.render_mode << "\n";
			if (camera->following == e.id && camera->fps) return; // Do not render yourself in FPS mode
			if (pos.z > camera_position->region_z - 10 && pos.z <= camera_position->region_z) {
				switch (r.render_mode) {
				case RENDER_SETTLER: render_settler(e, r, pos); break;
				case RENDER_SENTIENT: render_composite_sentient(e, r, pos); break;
				}
			}
		});
	}

	static void build_chunk_models() {
		for (const auto &idx : visible_chunks) {
			const auto * target = &chunks::chunks[idx.chunk_id];
			if (!target->static_voxel_models.empty()) {
				for (auto &model : target->static_voxel_models) {
					for (auto &pos : model.second) {
						if (std::get<2>(pos) <= camera_position->region_z) {
							const auto &[a, c, b] = pos;
							add_voxel_model(model.first, static_cast<float>(a), static_cast<float>(b), static_cast<float>(c), 1.0f, 1.0f, 1.0f);
						}
					}
				}
			}
		}
	}

	static void build_creature_models() {
		bengine::each<position_t, renderable_t, grazer_ai>([](bengine::entity_t &e, position_t &pos, renderable_t &r, grazer_ai &g) {
			const int chunkidx = chunks::chunk_id_by_world_pos(pos.x, pos.y, pos.z);
			if (visible_chunk_set.find(chunkidx) != visible_chunk_set.end()) {
				if (r.vox != 0) {
					//std::cout << "Found critter " << r.vox << "\n";
					const auto is_upright = true;
					const auto rotation = is_upright ? (static_cast<float>(pos.rotation) * 3.14159265358979323846f) / 180.0f : 1.5707963267948966192313216916398f;
					const auto rot1 = is_upright ? 0.0f : 1.0f;
					const auto rot2 = is_upright ? 1.0f : 0.0f;
					const auto rot3 = 0.0f;
					add_voxel_model(r.vox, static_cast<float>(pos.x), static_cast<float>(pos.z), static_cast<float>(pos.y), 1.0f, 1.0f, 1.0f, rotation, rot1, rot2, rot3);
				}
			}
		});

		// Render sentients who don't have a composite component
		bengine::each_without<renderable_composite_t, position_t, sentient_ai, species_t>([](bengine::entity_t &e, position_t &pos, sentient_ai &g, species_t &species) {
			auto def = get_species_def(species.tag);
			if (def == nullptr) return;

			if (def->voxel_model != 0) {
				const int chunkidx = chunks::chunk_id_by_world_pos(pos.x, pos.y, pos.z);
				if (visible_chunk_set.find(chunkidx) != visible_chunk_set.end()) {
					//std::cout << "Found critter " << r.vox << "\n";
					const auto is_upright = true;
					const auto rotation = is_upright ? (static_cast<float>(pos.rotation) * 3.14159265358979323846f) / 180.0f : 1.5707963267948966192313216916398f;
					const auto rot1 = is_upright ? 0.0f : 1.0f;
					const auto rot2 = is_upright ? 1.0f : 0.0f;
					const auto rot3 = 0.0f;

					add_voxel_model(def->voxel_model, static_cast<float>(pos.x), static_cast<float>(pos.z), static_cast<float>(pos.y), 1.0f, 1.0f, 1.0f, rotation, rot1, rot2, rot3);
				}
			}
		});
	}

	void build_voxel_render_list() {
		if (sprite_vao == 0) glGenVertexArrays(1, &sprite_vao);
		if (sprite_vbo == 0) glGenBuffers(1, &sprite_vbo);
		if (glyph_vao == 0) glGenVertexArrays(1, &glyph_vao);
		if (glyph_vbo == 0) glGenBuffers(1, &glyph_vbo);

		if (!models_to_render) {
			models_to_render = std::make_unique<boost::container::flat_map<int, std::vector<vox::instance_t>>>();
		}

		if (models_changed) {
			models_to_render->clear();
			model_buffers.clear();

			build_chunk_models();
			build_voxel_buildings();
			build_voxel_items();
			build_creature_models();
			// Regular old renderables!
			

			build_composites();

			models_changed = false;

			vox::start_buffer_accumulation();
			for (auto &m : *models_to_render) {
				auto model = vox::get_model(m.first);
				auto mb = std::make_unique<vox::voxel_render_buffer_t>();
				model->build_buffer(m.second, mb.get());
				model_buffers.emplace_back(std::move(mb));
			}
			vox::finish_instance_buffers();
		}
	}

	void render_voxel_models(gbuffer_t * gbuffer, glm::mat4 &camera_projection_matrix, glm::mat4 &camera_modelview_matrix) {
		vox::bulk_render(model_buffers);
	}
}
