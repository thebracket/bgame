#include "stdafx.h"
#include "renderables.hpp"
#include "../../global_assets/game_camera.hpp"
#include "../../components/position.hpp"
#include "../../components/building.hpp"
#include "../../components/renderable_composite.hpp"
#include "../../components/species.hpp"
#include "../../components/item.hpp"
#include "../../components/item_carried.hpp"
#include "../../components/renderable.hpp"
#include "../../components/grazer_ai.hpp"
#include "../../components/sentient_ai.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "voxel_model.hpp"
#include "voxreader.hpp"
#include "../../global_assets/shader_storage.hpp"
#include "../../global_assets/texture_storage.hpp"
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

namespace render {

	bool models_changed = true;
	static std::unique_ptr<boost::container::flat_map<int, std::vector<vox::instance_t>>> models_to_render;
	static std::vector<std::unique_ptr<vox::voxel_render_buffer_t>> model_buffers;
	static std::vector<std::tuple<int, int, int, bengine::color_t, uint16_t>> glyphs;
	static std::vector<float> glyph_buffer;
	static unsigned int sprite_vao = 0;
	static unsigned int sprite_vbo = 0;
	static unsigned int glyph_vao = 0;
	static unsigned int glyph_vbo = 0;

	static void add_voxel_model(const int &model, const float &x, const float &y, const float &z, const float &red, const float &green, const float &blue, float angle=0.0f, float x_rot=0.0f, float y_rot=0.0f, float z_rot=0.0f) {
		auto finder = models_to_render->find(model);
		if (finder != models_to_render->end()) {
			finder->second.push_back(vox::instance_t{x, y, z, angle, x_rot, y_rot, z_rot, red, green, blue});
		}
		else {
			models_to_render->insert(std::make_pair(model, std::vector<vox::instance_t>{vox::instance_t{x, y, z, x_rot, y_rot, z_rot, angle, red, green, blue}}));
		}
	}

	static void build_voxel_buildings() {
		bengine::each<building_t, position_t>(
			[](bengine::entity_t &e, building_t &b, position_t &pos) {
			if (pos.z > camera_position->region_z - 10 && pos.z <= camera_position->region_z) {
				if (b.vox_model > 0) {
					//std::cout << "Found model #" << b.vox_model << "\n";
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

					add_voxel_model(b.vox_model, x, y, z, red, green, blue);
				} // TODO: Add ASCII support
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

				auto x = (float)systems::mouse_wx;
				const auto y = (float)systems::mouse_wz;
				auto z = (float)systems::mouse_wy;

				if (can_build) {					

					add_voxel_model(building_def->vox_model, x, y, z, 1.0f, 1.0f, 1.0f);

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
					add_voxel_model(building_def->vox_model, x, y, z, 1.0f, 0.0f, 0.0f);
				}
			}
		}
	}

	static void build_voxel_items() {
		bengine::each<item_t, position_t>([](bengine::entity_t &e, item_t &i, position_t &pos) {
			auto item_def = get_item_def(i.item_tag);
			if (pos.z > camera_position->region_z - 10 && pos.z <= camera_position->region_z && item_def) {
				if (item_def->voxel_model > 0) {
					auto x = (float)pos.x;
					const auto y = (float)pos.z;
					auto z = (float)pos.y;
					add_voxel_model(item_def->voxel_model, x, y, z, 1.0f, 1.0f, 1.0f);
				}
				else if (item_def->glyph > 0) {
					glyphs.push_back(std::tuple<int, int, int, bengine::color_t, uint16_t>{ pos.x, pos.y, pos.z, item_def->fg, item_def->glyph_ascii });
				}
			}
		});
	}

	static void render_settler(bengine::entity_t &e, renderable_composite_t &r, position_t &pos) {
		auto species = e.component<species_t>();
		if (!species) return;

		// For now, everything uses voxel 49!
		if (pos.z > camera_position->region_z - 10 && pos.z <= camera_position->region_z) {
			const float X = static_cast<float>(pos.x);
			const float Y = static_cast<float>(pos.z);
			const float Z = static_cast<float>(pos.y);
			const float rotation = (static_cast<float>(pos.rotation) * 3.14159265358979323846f) / 180.0f;

			// Clip check passed - add the model
			add_voxel_model(49, X, Y, Z, species->skin_color.second.r, species->skin_color.second.g, species->skin_color.second.b, rotation, 0.0f, 1.0f, 0.0f);

			// Add hair
			int hair_vox;
			switch (species->hair_style) {
				case SHORT_HAIR : hair_vox = 50; break;
				case LONG_HAIR: hair_vox = 51; break;
				case PIGTAILS: hair_vox = 52; break;
				case MOHAWK: hair_vox = 53; break;
				case BALDING: hair_vox = 54; break;
				case TRIANGLE: hair_vox = 55; break;
				default : hair_vox = 0;
			}
			if (hair_vox > 0) {
				add_voxel_model(hair_vox, X, Y, Z, species->hair_color.second.r, species->hair_color.second.g, species->hair_color.second.b, rotation, 0.0f, 1.0f, 0.0f);
			}

			// Add items
			using namespace bengine;
			each<item_t, item_carried_t>([&pos, &e, &X, &Y, &Z, &rotation](entity_t &E, item_t &item, item_carried_t &carried) {
				if (carried.carried_by == e.id && item.clothing_layer > 0) {
					add_voxel_model(item.clothing_layer, X, Y, Z, item.clothing_color.r, item.clothing_color.g, item.clothing_color.b, rotation, 0.0f, 1.0f, 0.0f);
				}
			});
		}
	}

	static void render_composite_sentient(bengine::entity_t &e, renderable_composite_t &r, position_t &pos) {
		// TODO: This should follow a different code path
		//std::cout << "Render sentient\n";

		render_settler(e, r, pos);
	}

	static void build_composites() {
		bengine::each<renderable_composite_t, position_t>([](bengine::entity_t &e, renderable_composite_t &r, position_t &pos) {
			//std::cout << r.render_mode << "\n";
			if (pos.z > camera_position->region_z - 10 && pos.z <= camera_position->region_z) {
				switch (r.render_mode) {
				case RENDER_SETTLER: render_settler(e, r, pos); break;
				case RENDER_SENTIENT: render_composite_sentient(e, r, pos); break;
				}
			}
		});
	}

	static void build_chunk_models(const boost::container::flat_set<int, std::greater<int>> &visible_chunks) {
		for (const auto &idx : visible_chunks) {
			chunks::chunk_t * target = &chunks::chunks[idx];
			if (!target->static_voxel_models.empty()) {
				for (auto &model : target->static_voxel_models) {
					if (std::get<2>(model.second) <= camera_position->region_z) {
						auto finder = models_to_render->find(model.first);
						const auto &[a, c, b] = model.second;
						vox::instance_t render_model{ static_cast<float>(a), static_cast<float>(b), static_cast<float>(c), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
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

	static void build_creature_models() {
		bengine::each<position_t, renderable_t, grazer_ai>([](bengine::entity_t &e, position_t &pos, renderable_t &r, grazer_ai &g) {
			if (r.vox != 0) {
				//std::cout << "Found critter " << r.vox << "\n";
				auto finder = models_to_render->find(r.vox);
				vox::instance_t render_model{ static_cast<float>(pos.x), static_cast<float>(pos.z), static_cast<float>(pos.y), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
				if (finder != models_to_render->end()) {
					finder->second.push_back(render_model);
				}
				else {
					models_to_render->insert(std::make_pair(r.vox, std::vector<vox::instance_t>{ render_model }));
				}
			}
			else {
				glyphs.push_back(std::tuple<int, int, int, bengine::color_t, uint16_t>{ static_cast<float>(pos.x), static_cast<float>(pos.z), static_cast<float>(pos.y), r.foreground, r.glyph_ascii });
			}
		});

		// Render sentients who don't have a composite component
		bengine::each<position_t, sentient_ai, species_t>([](bengine::entity_t &e, position_t &pos, sentient_ai &g, species_t &species) {
			if (e.component<renderable_composite_t>() == nullptr) {
				auto def = get_species_def(species.tag);
				if (def == nullptr) return;

				if (def->voxel_model != 0) {
					//std::cout << "Found critter " << r.vox << "\n";
					auto finder = models_to_render->find(def->voxel_model);
					vox::instance_t render_model{ static_cast<float>(pos.x), static_cast<float>(pos.z), static_cast<float>(pos.y), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
					if (finder != models_to_render->end()) {
						finder->second.push_back(render_model);
					}
					else {
						models_to_render->insert(std::make_pair(def->voxel_model, std::vector<vox::instance_t>{ render_model }));
					}
				}
			}
		});
	}

	void build_voxel_render_list(const boost::container::flat_set<int, std::greater<int>> &visible_chunks) {
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
			glyphs.clear();

			build_chunk_models(visible_chunks);
			build_voxel_buildings();
			build_voxel_items();
			build_creature_models();
			// Regular old renderables!
			

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

			// Build the glyphs buffer
			/*
			glyph_buffer.clear();
			for (const auto &g : glyphs) {
				// Insert the render info
				const auto&[x, z, y, color, ch] = g;

				constexpr float width = 1.0f;
				constexpr float height = 1.0f;
				const float x0 = -0.5f;
				const float x1 = x0 + width;
				const float y0 = -0.5f;
				const float y1 = y0 + 1.0f; // We don't use y1 for floors
				const float z0 = -0.5f;
				const float z1 = z0 + height;
				constexpr float ceiling_gap = 0.001f;

				const float posx = static_cast<float>(x);
				const float posy = static_cast<float>(z);
				const float posz = static_cast<float>(y);

				const float char_x = static_cast<float>(ch % 16) * 1024.0f;
				const float char_y = static_cast<float>(ch / 16) * 1024.0f;
				constexpr float char_size_gl = 16.0f / 1024.0f;
				const float T0 = char_x / 1024.0f;
				const float T1 = char_y / 1024.0f;
				const float TW = char_size_gl;
				const float TH = char_size_gl;

				glyph_buffer.insert(sprite_buffer.end(), {
					// Upwards facing floor; this will need to change per camera type
					x0, y0, z1, T0, T1, 0.0f,  0.0f, 1.0f, color.r, color.g, color.b, posx, posy, posz,
					x1, y0, z1, TW, T1, 0.0f,  0.0f, 1.0f, color.r, color.g, color.b, posx, posy, posz,
					x1, y1, z1, TW, TH, 0.0f,  0.0f, 1.0f, color.r, color.g, color.b, posx, posy, posz,
					x1, y1, z1, TW, TH, 0.0f,  0.0f, 1.0f, color.r, color.g, color.b, posx, posy, posz,
					x0, y1, z1, T0, TH, 0.0f,  0.0f, 1.0f, color.r, color.g, color.b, posx, posy, posz,
					x0, y0, z1, T0, T1, 0.0f,  0.0f, 1.0f, color.r, color.g, color.b, posx, posy, posz
				});
			}
			glBindVertexArray(glyph_vao);
			glBindBuffer(GL_ARRAY_BUFFER, glyph_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * glyph_buffer.size(), &glyph_buffer[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, glyph_vao);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0); // 0 = Vertex Position

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (char *) nullptr + 3 * sizeof(float));
			glEnableVertexAttribArray(1); // 1 = TexX/Y

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (char *) nullptr + 5 * sizeof(float));
			glEnableVertexAttribArray(2); // 2 = Normals

			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (char *) nullptr + 8 * sizeof(float));
			glEnableVertexAttribArray(3); // 3 = Colors

			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (char *) nullptr + 11 * sizeof(float));
			glEnableVertexAttribArray(4); // 4 = Translate

			glBindVertexArray(0);*/

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
	}

	void render_voxel_models_shadow(float &radius, glm::vec3 &light_pos, std::vector<glm::mat4> &shadowTransforms, unsigned int textureId) {
		assets::voxel_shadow_shader->use();
		//glBindFramebuffer(GL_FRAMEBUFFER, light_stage_buffer->fbo_id);
		for (int i = 0; i < 6; ++i) {
			glUniformMatrix4fv(assets::voxel_shadow_shader->combined_matrix, 1, GL_FALSE, glm::value_ptr(shadowTransforms[i]));
			glUniform1f(assets::voxel_shadow_shader->texSize, 32.0f);
			glUniform1f(assets::voxel_shadow_shader->far_plane, radius);
			glUniform3f(assets::voxel_shadow_shader->lightPos, light_pos.x, light_pos.y, light_pos.z);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textureId, 0);

			for (const auto &m : model_buffers) {
				m->model->render_instances(*m);
			}
		}
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureId, 0);
	}
}