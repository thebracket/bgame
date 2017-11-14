#include "renderables.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../global_assets/game_camera.hpp"
#include "../../bengine/ecs.hpp"
#include "../../components/position.hpp"
#include "../../components/building.hpp"
#include "../../components/renderable_composite.hpp"
#include "../../components/species.hpp"
#include "../../components/item.hpp"
#include "../../components/item_carried.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "voxel_model.hpp"
#include "voxreader.hpp"
#include "../../global_assets/shader_storage.hpp"
#include "../../global_assets/texture_storage.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../global_assets/game_building.hpp"
#include "../../raws/buildings.hpp"
#include "../../raws/defs/building_def_t.hpp"
#include "../../systems/mouse.hpp"
#include "../../planet/region/region.hpp"
#include "../../systems/ai/inventory_system.hpp"
#include "../chunks/chunks.hpp"
#include "../../stdafx.h"

namespace render {

	bool models_changed = true;
	std::unique_ptr<boost::container::flat_map<int, std::vector<vox::instance_t>>> models_to_render;
	std::vector<std::unique_ptr<vox::voxel_render_buffer_t>> model_buffers;
	std::vector<float> sprite_buffer;

	void build_voxel_buildings() {
		bengine::each<building_t, position_t>(
			[](bengine::entity_t &e, building_t &b, position_t &pos) {
			if (b.vox_model > 0 && pos.z > camera_position->region_z - 10 && pos.z <= camera_position->region_z) {
				//std::cout << "Found model #" << b.vox_model << "\n";
				auto finder = models_to_render->find(b.vox_model);
				auto x = (float)pos.x;
				const auto y = (float)pos.z;
				auto z = (float)pos.y;

				//std::cout << b.width << " x " << b.height << "\n";
				if (b.width == 3) x -= 1.0f;
				if (b.height == 3) z -= 1.0f;

				float red = 1.0f;
				float green = 1.0f;
				float blue = 1.0f;

				if (!b.complete) {
					red = 0.1f;
					green = 0.1f;
					blue = 0.1f;
				}

				if (finder != models_to_render->end()) {
					finder->second.push_back(vox::instance_t{
						x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, red, green, blue
					});
				}
				else {
					models_to_render->insert(std::make_pair(b.vox_model, std::vector<vox::instance_t>{vox::instance_t{
						x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, red, green, blue
					}}));
				}
			}
		});

		if (game_master_mode == DESIGN && game_design_mode == BUILDING && buildings::has_build_mode_building) {
			// We have a building selected; determine if it can be built and show it
			const auto tag = buildings::build_mode_building.tag;
			auto building_def = get_building_def(tag);
			if (building_def && building_def->vox_model > 0) {
				// We have the model and the definition; see if its possible to build
				bool can_build = true;

				std::vector<int> target_tiles;
				for (int y = systems::mouse_wy; y < systems::mouse_wy + building_def->height; ++y) {
					for (int x = systems::mouse_wx; x < systems::mouse_wx + building_def->width; ++x) {
						const auto idx = mapidx(x, y, systems::mouse_wz);
						if (!region::flag(idx, CAN_STAND_HERE)) can_build = false;
						if (region::flag(idx, CONSTRUCTION)) can_build = false;
						target_tiles.emplace_back(idx);
					}
				}

				auto finder = models_to_render->find(building_def->vox_model);
				auto x = (float)systems::mouse_wx;
				const auto y = (float)systems::mouse_wz;
				auto z = (float)systems::mouse_wy;

				if (can_build) {					

					if (finder != models_to_render->end()) {
						finder->second.push_back(vox::instance_t{
							x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
						});
					}
					else {
						models_to_render->insert(std::make_pair(building_def->vox_model, std::vector<vox::instance_t>{ vox::instance_t{
							x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
						} }));
					}

					if (systems::left_click) {
						// Perform the building
						systems::inventory_system::building_request(systems::mouse_wx, systems::mouse_wy, systems::mouse_wz, buildings::build_mode_building);
						buildings::has_build_mode_building = false;
						for (const auto &idx : target_tiles) {
							region::set_flag(idx, CONSTRUCTION);
						}
					}
				}
				else {
					if (finder != models_to_render->end()) {
						finder->second.push_back(vox::instance_t{
							x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
						});
					}
					else {
						models_to_render->insert(std::make_pair(building_def->vox_model, std::vector<vox::instance_t>{ vox::instance_t{
							x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
						} }));
					}
				}
			}
		}
	}

	void build_voxel_items() {
		bengine::each<item_t, position_t>([](bengine::entity_t &e, item_t &i, position_t &pos) {
			auto item_def = get_item_def(i.item_tag);
			if (pos.z > camera_position->region_z - 10 && pos.z <= camera_position->region_z && item_def && item_def->voxel_model > 0) {
				auto finder = models_to_render->find(item_def->voxel_model);
				auto x = (float)pos.x;
				const auto y = (float)pos.z;
				auto z = (float)pos.y;
				if (finder != models_to_render->end()) {
					finder->second.push_back(vox::instance_t{
						x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
					});
				}
				else {
					models_to_render->insert(std::make_pair(item_def->voxel_model, std::vector<vox::instance_t>{vox::instance_t{
						x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
					}}));
				}
			}
		});
	}

	int n_sprites = 0;

	void add_sprite(const position_t &pos, const int &sprite_id, const bengine::color_t &color) {
		constexpr float width = 1.0f;
		constexpr float height = 1.0f;
		const float x0 = -0.5f;
		const float x1 = x0 + width;
		const float y0 = -0.5f;
		const float y1 = y0 + 1.0f; // We don't use y1 for floors
		const float z0 = -0.5f;
		const float z1 = z0 + height;
		const float TI = sprite_id;
		constexpr float T0 = 0.0f;
		const float TW = width;
		const float TH = height;
		constexpr float ceiling_gap = 0.001f;

		const float posx = static_cast<float>(pos.x);
		const float posy = static_cast<float>(pos.z);
		const float posz = static_cast<float>(pos.y);

		sprite_buffer.insert(sprite_buffer.end(), {
			// Upwards facing floor; this will need to change per camera type
			x0, y0, z1, T0, T0, TI,  0.0f,  0.0f, 1.0f, color.r, color.g, color.b, posx, posy, posz,
			x1, y0, z1, TW, T0, TI,  0.0f,  0.0f, 1.0f, color.r, color.g, color.b, posx, posy, posz,
			x1, y1, z1, TW, TH, TI,  0.0f,  0.0f, 1.0f, color.r, color.g, color.b, posx, posy, posz,
			x1, y1, z1, TW, TH, TI,  0.0f,  0.0f, 1.0f, color.r, color.g, color.b, posx, posy, posz,
			x0, y1, z1, T0, TH, TI,  0.0f,  0.0f, 1.0f, color.r, color.g, color.b, posx, posy, posz,
			x0, y0, z1, T0, T0, TI,  0.0f,  0.0f, 1.0f, color.r, color.g, color.b, posx, posy, posz
		});

		++n_sprites;
	}

	void render_settler(bengine::entity_t &e, renderable_composite_t &r, position_t &pos) {
		using namespace bengine;

		// TODO: Add sprite
		auto species = e.component<species_t>();
		if (!species) return;

		// Gender-based base sprite
		switch (species->gender) {
		case MALE: { 
			add_sprite(pos, 0, species->skin_color.second); 
			add_sprite(pos, 2, bengine::color_t( 1.0f, 1.0f, 1.0f ));
		} break;
		case FEMALE: { 
			add_sprite(pos, 1, species->skin_color.second); 
			add_sprite(pos, 3, bengine::color_t(1.0f, 1.0f, 1.0f));
			add_sprite(pos, 4, bengine::color_t(1.0f, 1.0f, 1.0f));
		} break;
		}

		// Hair
		switch (species->hair_style) {
		case SHORT_HAIR: add_sprite(pos, 24, species->hair_color.second); break;
		case LONG_HAIR: add_sprite(pos, 25, species->hair_color.second); break;
		case PIGTAILS: add_sprite(pos, 26, species->hair_color.second); break;
		case MOHAWK: add_sprite(pos, 27, species->hair_color.second); break;
		case TRIANGLE: add_sprite(pos, 25, species->hair_color.second); break;
		}

		// Items/Clothing
		each<item_t, item_carried_t>([&pos, &e](entity_t &E, item_t &item, item_carried_t &carried) {
			if (carried.carried_by == e.id && item.clothing_layer > 0) {
				add_sprite(pos, item.clothing_layer, item.clothing_color);
			}
		});
	}

	void render_sentient(bengine::entity_t &e, renderable_composite_t &r, position_t &pos) {
		// TODO: Add sprite
	}

	void build_composites() {
		bengine::each<renderable_composite_t, position_t>([](bengine::entity_t &e, renderable_composite_t &r, position_t &pos) {
			if (pos.z > camera_position->region_z - 10 && pos.z <= camera_position->region_z) {
				switch (r.render_mode) {
				case RENDER_SETTLER: render_settler(e, r, pos); break;
				case RENDER_SENTIENT: render_sentient(e, r, pos); break;
				}
			}
		});
	}

	unsigned int sprite_vao = 0;
	unsigned int sprite_vbo = 0;

	void build_chunk_models(const boost::container::flat_set<int, std::greater<int>> &visible_chunks) {
		for (const auto &idx : visible_chunks) {
			chunks::chunk_t * target = &chunks::chunks[idx];
			if (!target->static_voxel_models.empty()) {
				for (const auto &model : target->static_voxel_models) {
					if (std::get<2>(model.second) <= camera_position->region_z) {
						auto finder = models_to_render->find(model.first);
						vox::instance_t render_model{ std::get<0>(model.second), std::get<2>(model.second), std::get<1>(model.second), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
						if (finder != models_to_render->end()) {
							finder->second.push_back(render_model);
						}
						else {
							models_to_render->insert(std::make_pair(model.first, std::vector<vox::instance_t>{ render_model }));
						}
					}
				}
			}
		}
	}

	void build_voxel_render_list(const boost::container::flat_set<int, std::greater<int>> &visible_chunks) {
		if (sprite_vao == 0) glGenVertexArrays(1, &sprite_vao);
		if (sprite_vbo == 0) glGenBuffers(1, &sprite_vbo);

		if (!models_to_render) {
			models_to_render = std::make_unique<boost::container::flat_map<int, std::vector<vox::instance_t>>>();
		}

		if (models_changed) {
			models_to_render->clear();
			model_buffers.clear();
			sprite_buffer.clear();
			n_sprites = 0;
			
			build_chunk_models(visible_chunks);
			build_voxel_buildings();
			build_voxel_items();
			// TODO: Regular old renderables!

			build_composites();

			models_changed = false;

			assets::voxel_shader->use();
			for (auto &m : *models_to_render) {
				auto model = vox::get_model(m.first);
				auto mb = std::make_unique<vox::voxel_render_buffer_t>();
				model->build_buffer(m.second, mb.get());
				model_buffers.emplace_back(std::move(mb));
			}
			glUseProgram(0);

			// Build the sprite buffer
			glBindVertexArray(sprite_vao);
			glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sprite_buffer.size(), &sprite_buffer[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, sprite_vbo);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0); // 0 = Vertex Position

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (char *) nullptr + 3 * sizeof(float));
			glEnableVertexAttribArray(1); // 1 = TexX/Y/ID

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (char *) nullptr + 6 * sizeof(float));
			glEnableVertexAttribArray(2); // 2 = Normals

			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (char *) nullptr + 9 * sizeof(float));
			glEnableVertexAttribArray(3); // 3 = Colors

			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (char *) nullptr + 12 * sizeof(float));
			glEnableVertexAttribArray(4); // 4 = Translate

			glBindVertexArray(0);
		}
	}

	void render_voxel_models(gbuffer_t * gbuffer, glm::mat4 &camera_projection_matrix, glm::mat4 &camera_modelview_matrix) {
		assets::voxel_shader->use();
		glBindFramebuffer(GL_FRAMEBUFFER, gbuffer->fbo_id);
		glUniformMatrix4fv(assets::voxel_shader->projection_matrix, 1, GL_FALSE, glm::value_ptr(camera_projection_matrix));
		glUniformMatrix4fv(assets::voxel_shader->view_matrix, 1, GL_FALSE, glm::value_ptr(camera_modelview_matrix));
		glCheckError();
		glUniform1f(assets::voxel_shader->texSize, 32.0f);
		glCheckError();

		for (const auto &m : model_buffers) {
			m->model->render_instances(*m);
		}

		// Render the sprite buffer
		//std::cout << n_sprites << "\n";
		if (n_sprites > 0) {
			glDepthFunc(GL_GEQUAL);
			glUseProgram(assets::sprite_shader);
			glBindFramebuffer(GL_FRAMEBUFFER, gbuffer->fbo_id);
			glUniformMatrix4fv(glGetUniformLocation(assets::sprite_shader, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(camera_projection_matrix));
			glUniformMatrix4fv(glGetUniformLocation(assets::sprite_shader, "view_matrix"), 1, GL_FALSE, glm::value_ptr(camera_modelview_matrix));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D_ARRAY, assets::mob_texture_array);
			glUniform1i(glGetUniformLocation(assets::sprite_shader, "textureArray"), 0);
			glCheckError();
			glBindVertexArray(sprite_vao);
			glDrawArrays(GL_TRIANGLES, 0, sprite_buffer.size());
			glCheckError();
		}
	}
}