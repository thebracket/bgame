#include "renderables_system.hpp"
#include "../messages/renderables_changed_message.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../components/renderable.hpp"
#include "../components/renderable_composite.hpp"
#include "../components/position.hpp"
#include "../components/building.hpp"
#include "../planet/region.hpp"
#include "../main/game_globals.hpp"
#include "particle_system.hpp"
#include "../components/settler_ai.hpp"
#include "../components/construct_provides_door.hpp"
#include "../components/species.hpp"
#include "../components/item.hpp"

using namespace rltk;

boost::container::flat_map<int, std::vector<rltk::vchar>> renderables;
boost::container::flat_map<int, std::vector<std::vector<rltk::vchar>>> composite_renderables;

void renderables_system::configure() {
    system_name = "Renderables System";
    subscribe_mbox<renderables_changed_message>();
}

inline void add_render_composite(const std::size_t &id, const int &idx) {
    auto compr = entity(id)->component<renderable_composite_t>();
    if (!compr) return;

    std::vector<rltk::vchar> layers;

    if (compr->render_mode == RENDER_SETTLER) {
        auto species = entity(id)->component<species_t>();
        if (!species) return;

        // Render the base person glyph
        color_t skin_color;
        switch (species->skin_color) {
            case CAUCASIAN : skin_color = color_t(255,219,172); break;
            case ASIAN : skin_color = color_t(224,172,105); break;
            case INDIAN : skin_color = color_t(198,134,66); break;
            case AFRICAN : skin_color = color_t(141,85,36); break;
        }

        if (species->gender == MALE) {
            layers.push_back(vchar{352, skin_color, rltk::colors::BLACK});
        } else {
            layers.push_back(vchar{353, skin_color, rltk::colors::BLACK});
        }

        // Render hair and beard
        color_t hair_color;
        switch (species->hair_color) {
            case WHITE_HAIR : hair_color = color_t(250, 250, 250); break;
            case BROWN_HAIR : hair_color = color_t(141,85,36); break;
            case BLACK_HAIR : hair_color = color_t(50, 50, 64); break;
            case BLONDE_HAIR : hair_color = color_t(216, 192, 120); break;
            case RED_HAIR : hair_color = color_t(181, 82, 57); break;
        }

        int hair_glyph = 0;
        switch (species->hair_style) {
            case SHORT : hair_glyph = 354; break;
            case LONG : hair_glyph = 355; break;
            case PIGTAILS : hair_glyph = 356; break;
            case MOHAWK : hair_glyph = 357; break;
            case BALDING : hair_glyph = 358; break;
            case TRIANGLE : hair_glyph = 359; break;
            case BALD : hair_glyph = 0; break;
        }
        if (hair_glyph != 0) layers.push_back(vchar{hair_glyph, hair_color, rltk::colors::BLACK});
        if (species->bearded) layers.push_back(vchar{360, hair_color, rltk::colors::BLACK});

        // Render clothes
        each<item_carried_t, item_t>([&id, &layers] (entity_t &e, item_carried_t &c, item_t &i) {
            if (c.carried_by == id && c.location != INVENTORY && i.clothing_glyph != 0) {
                layers.push_back(vchar{i.clothing_glyph, i.clothing_color, rltk::colors::BLACK});
            }
        });
        composite_renderables[idx].push_back(layers);
    }
}

void renderables_system::update(const double time_elapsed) {
    std::queue<renderables_changed_message> * render_change = mbox<renderables_changed_message>();
	while (!render_change->empty()) {
		renderables_changed = true;
		render_change->pop();
	}

    if (renderables_changed) {
		renderables.clear();
        composite_renderables.clear();

		// Add buildings to renderables
		each<building_t, position_t>([] (entity_t &entity, building_t &b, position_t &pos) {
			int glyph_idx = 0;
			int offset_x = 0;
			int offset_y = 0;
			if (b.width == 3) offset_x = -1;
			if (b.height == 3) offset_y = -1;

			for (int y = 0; y<b.height; ++y) {
				for (int x=0; x<b.width; ++x) {
					const auto idx = mapidx(pos.x+x+offset_x, pos.y+y+offset_y, pos.z);
					rltk::vchar glyph;
					glyph = b.glyphs[glyph_idx];
					if (!b.complete) glyph.foreground = rltk::colors::GREY;
					auto door = entity.component<construct_door_t>();
					if (door && door->locked) glyph.background = rltk::colors::GREY;
					renderables[idx].push_back(glyph);
					++glyph_idx;
				}
			}
		});

		// Add other entities
		each<renderable_t, position_t>([] (entity_t &entity, renderable_t &render, position_t &pos) {
			renderables[mapidx(pos.x, pos.y, pos.z)].push_back(rltk::vchar{render.glyph, render.foreground, rltk::colors::BLACK});
		});

        // Add composite renderables
        each<renderable_composite_t, position_t>([] (entity_t &entity, renderable_composite_t &render, position_t &pos) {
            add_render_composite(entity.id, mapidx(pos));
        });

		// Add particles
		for (const particle_t &p : particles) {
			const auto idx = mapidx(p.x, p.y, p.z);
			renderables[idx].push_back(p.glyph);
		}

		// If we're rogue mode, and have a path, display it
		if (game_master_mode == ROGUE) {
			auto settler = entity(selected_settler);
			auto ai = settler->component<settler_ai_t>();
			if (ai->current_path && ai->current_path->success) {
				const float n_steps = ai->current_path->steps.size();
				float i = 0;
				for (auto step : ai->current_path->steps) {
					const float lerp_amount = i / n_steps;
					const auto idx = mapidx(step.x, step.y, step.z);
					renderables[idx].push_back(rltk::vchar{ 177, lerp(rltk::colors::DARK_GREEN, rltk::colors::LIGHTEST_GREEN, lerp_amount), rltk::colors::BLACK });
					++i;
				}
			}
		}
		
        emit(map_rerender_message{});
	}
}