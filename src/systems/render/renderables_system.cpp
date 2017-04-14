#include "renderables_system.hpp"
#include "../../messages/renderables_changed_message.hpp"
#include "../../messages/map_dirty_message.hpp"
#include "../../components/renderable.hpp"
#include "../../components/renderable_composite.hpp"
#include "../../components/position.hpp"
#include "../../components/building.hpp"
#include "../../planet/region.hpp"
#include "../../main/game_globals.hpp"
#include "../gui/particle_system.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/construct_provides_door.hpp"
#include "../../components/species.hpp"
#include "../../components/item.hpp"
#include "../ai/movement_system.hpp"
#include "../../raws/species.hpp"
#include "../../components/sleep_clock_t.hpp"

using namespace rltk;

spp::sparse_hash_map<int, std::vector<screen_render_t>> renderables;
spp::sparse_hash_map<int, std::vector<std::vector<screen_render_t>>> composite_renderables;

void renderables_system::configure() {
    system_name = "Renderables System";
    subscribe_mbox<renderables_changed_message>();
}

inline void add_render_composite(const std::size_t &id, const int &idx) {
    auto compr = entity(id)->component<renderable_composite_t>();
    auto pos = entity(id)->component<position_t>();
    if (!compr || !pos) return;

    if (camera->ascii_mode) {
        composite_renderables[idx].emplace_back(std::vector<screen_render_t>{ screen_render_t{ (float)pos->x, (float)pos->y, pos->offsetX, pos->offsetY, vchar{ compr->ascii_char, colors::WHITE, colors::BLACK } } });
        return;
    }

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
            case SHORT_HAIR : hair_glyph = 354; break;
            case LONG_HAIR : hair_glyph = 355; break;
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

        // Optionally render a status flag
        auto ai = entity(id)->component<settler_ai_t>();
        if (ai) {
            if (ai->job_type_major == JOB_MINE) {
                layers.push_back(vchar{337, rltk::colors::WHITE, rltk::colors::BLACK});
            } else if (ai->job_type_major == JOB_CHOP) {
                layers.push_back(vchar{338, rltk::colors::WHITE, rltk::colors::BLACK});
            } else if (ai->job_type_major == JOB_CONST || ai->job_type_major == JOB_ARCHITECT || ai->job_type_major == JOB_CARPENTRY) {
                layers.push_back(vchar{339, rltk::colors::WHITE, rltk::colors::BLACK});
            }
        }

        auto sleep = entity(id)->component<sleep_clock_t>();
        if (sleep && sleep->is_sleeping) {
            layers.push_back(vchar{336, rltk::colors::WHITE, rltk::colors::BLACK});
        }

        std::vector<screen_render_t> tmp;
        for (const vchar &c : layers) {
            tmp.push_back(screen_render_t{(float)pos->x, (float)pos->y, pos->offsetX, pos->offsetY, c});
        }
        composite_renderables[idx].push_back(tmp);
    } else if (compr->render_mode == RENDER_SENTIENT) {
        auto species = entity(id)->component<species_t>();
        if (!species) return;
        auto species_f = get_species_def(species->tag);

        // Render the base person glyph
        color_t skin_color;
        switch (species->skin_color) {
            case CAUCASIAN : skin_color = color_t(255,219,172); break;
            case ASIAN : skin_color = color_t(224,172,105); break;
            case INDIAN : skin_color = color_t(198,134,66); break;
            case AFRICAN : skin_color = color_t(141,85,36); break;
        }

        if (species->gender == MALE) {
            layers.push_back(vchar{species_f->base_male_glyph, skin_color, rltk::colors::BLACK});
        } else {
            layers.push_back(vchar{species_f->base_female_glyph, skin_color, rltk::colors::BLACK});
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
            case SHORT_HAIR : hair_glyph = 354; break;
            case LONG_HAIR : hair_glyph = 355; break;
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
            if (c.carried_by == id && i.clothing_glyph != 0) {
                layers.push_back(vchar{i.clothing_glyph, i.clothing_color, rltk::colors::BLACK});
            }
        });

        std::vector<screen_render_t> tmp;
        for (const vchar &c : layers) {
            tmp.push_back(screen_render_t{(float)pos->x, (float)pos->y, pos->offsetX, pos->offsetY, c});
        }
        composite_renderables[idx].push_back(tmp);
    }
}

void renderables_system::update(const double time_elapsed) {
    std::queue<renderables_changed_message> * render_change = mbox<renderables_changed_message>();
	while (!render_change->empty()) {
		renderables_changed = true;
		render_change->pop();
	}
    renderables_changed = true;

    if (renderables_changed) {
        renderables.clear();
        composite_renderables.clear();

        std::vector<std::size_t> visible_here = entity_octree.find_by_region(clip_left-1, clip_right+1, clip_top-1, clip_bottom+1, camera_position->region_z+1, camera_position->region_z-6);

        for (const std::size_t &id : visible_here) {
            auto E = entity(id);
            if (!E) break;

            bool done = false;
            auto b = E->component<building_t>();
            auto pos = E->component<position_t>();
            if (!pos) return;
            const int idx = mapidx(*pos);
            if (!done && b && pos) {
                int glyph_idx = 0;
                int offset_x = 0;
                int offset_y = 0;
                if (b->width == 3) offset_x = -1;
                if (b->height == 3) offset_y = -1;

                for (int y = 0; y<b->height; ++y) {
                    for (int x=0; x<b->width; ++x) {
                        const auto idx = mapidx(pos->x + offset_x, pos->y + offset_y, pos->z);
                        rltk::vchar glyph;
                        glyph = camera->ascii_mode && b->glyphs_ascii.size() == b->glyphs.size() ? b->glyphs_ascii[glyph_idx] : b->glyphs[glyph_idx];
                        if (b->glyphs_ascii.size() != b->glyphs.size()) std::cout << "WARNING: " << b->tag << " ASCII information invalid.\n";
                        if (!b->complete) glyph.foreground = rltk::colors::GREY;
                        auto door = E->component<construct_door_t>();
                        if (door && door->locked) glyph.background = rltk::colors::GREY;
                        renderables[idx].push_back(screen_render_t{pos->x + offset_x, pos->y + offset_y, pos->offsetX, pos->offsetY, glyph});
                        ++glyph_idx;
                        ++offset_x;
                    }
                    offset_x = 0;
                    if (b->width == 3) offset_x = -1;
                    ++offset_y;
                }

                done = true;
            }
            if (!done && E->component<renderable_composite_t>()) {
                add_render_composite(E->id, idx);
                done = true;
            }
            if (!done && pos) {
                auto render = E->component<renderable_t>();
                if (render) {
                    if (!camera->ascii_mode) {
                            renderables[idx].push_back(
                                screen_render_t{(float)pos->x, (float)pos->y, pos->offsetX, pos->offsetY,
                                            rltk::vchar{render->glyph, render->foreground, rltk::colors::BLACK}});
                        } else {
                        renderables[idx].push_back(
                                screen_render_t{(float)pos->x, (float)pos->y, pos->offsetX, pos->offsetY,
                                                rltk::vchar{render->glyph_ascii, render->foreground, rltk::colors::BLACK}});
                    }
                }
            }
        }

		// Add particles
		for (const particle_t &p : particles) {
            if (p.x > 0 && p.x < REGION_WIDTH-1 && p.y > 0 && p.y < REGION_HEIGHT-1 && p.z > 0 && p.z < REGION_DEPTH-1) {
                const auto idx = mapidx(p.x, p.y, p.z);
                renderables[idx].push_back(screen_render_t{p.x, p.y, 0, 0, p.glyph});
            }
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
					renderables[idx].push_back(screen_render_t{step.x, step.y, 0, 0, rltk::vchar{ 177, lerp(rltk::colors::DARK_GREEN, rltk::colors::LIGHTEST_GREEN, lerp_amount), rltk::colors::BLACK }});
					++i;
				}
			}
		}
		
        emit(map_rerender_message{});
	}
}
