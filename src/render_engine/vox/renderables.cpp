#include "renderables.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../global_assets/game_camera.hpp"
#include "../../bengine/ecs.hpp"
#include "../../components/position.hpp"
#include "../../components/building.hpp"
#include "../../components/renderable_composite.hpp"
#include "../../components/species.hpp"
#include "voxel_model.hpp"
#include "voxreader.hpp"
#include "../../global_assets/shader_storage.hpp"
#include <memory>
#include <vector>
#include <boost/container/flat_map.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace render {

	bool models_changed = true;
	std::unique_ptr<boost::container::flat_map<int, std::vector<vox::instance_t>>> models_to_render;
	std::vector<std::unique_ptr<vox::voxel_render_buffer_t>> model_buffers;

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

				if (finder != models_to_render->end()) {
					finder->second.push_back(vox::instance_t{
						x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
					});
				}
				else {
					models_to_render->insert(std::make_pair(b.vox_model, std::vector<vox::instance_t>{vox::instance_t{
						x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
					}}));
				}
			}
		});
	}

	void add_composite_layer(const int model_id, const position_t &pos, const bengine::color_t &color) {
		auto finder = models_to_render->find(model_id);
		auto x = (float)pos.x;
		const auto y = (float)pos.z;
		auto z = (float)pos.y;
		if (finder != models_to_render->end()) {
			finder->second.push_back(vox::instance_t{ x, y, z, 0.0f, 1.0f, 0.0f, pos.rotation * 0.0174533f, color.r, color.g, color.b });
		}
		else {
			models_to_render->insert(std::make_pair(model_id, std::vector<vox::instance_t>{vox::instance_t{
				x, y, z, 0.0f, 1.0f, 0.0f, pos.rotation * 0.0174533f, color.r, color.g, color.b
			}}));
		}
	}

	void render_settler(bengine::entity_t &e, renderable_composite_t &r, position_t &pos) {
		// Render the torso, with skin tinted appropriately
		auto species = e.component<species_t>();
		if (!species) return;

		add_composite_layer(7, pos, species->skin_color.second);

		switch (species->hair_style) {
		case SHORT_HAIR: add_composite_layer(10, pos, species->hair_color.second);
		case LONG_HAIR: add_composite_layer(11, pos, species->hair_color.second);
		case PIGTAILS: add_composite_layer(12, pos, species->hair_color.second);
		case MOHAWK: add_composite_layer(13, pos, species->hair_color.second);
		case BALDING: add_composite_layer(10, pos, species->hair_color.second);
		case TRIANGLE: add_composite_layer(10, pos, species->hair_color.second);
		default: {}
		}
	}

	void render_sentient(bengine::entity_t &e, renderable_composite_t &r, position_t &pos) {

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

	void build_voxel_render_list() {
		if (!models_to_render) {
			models_to_render = std::make_unique<boost::container::flat_map<int, std::vector<vox::instance_t>>>();
		}

		if (models_changed) {
			models_to_render->clear();
			model_buffers.clear();
			
			build_voxel_buildings();
			// TODO: Regular old renderables!

			//build_composites();

			models_changed = false;

			assets::voxel_shader->use();
			for (auto &m : *models_to_render) {
				auto model = vox::get_model(m.first);
				auto mb = std::make_unique<vox::voxel_render_buffer_t>();
				model->build_buffer(m.second, mb.get());
				model_buffers.emplace_back(std::move(mb));
			}
			glUseProgram(0);
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
}