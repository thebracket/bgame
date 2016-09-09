#include "renderables_system.hpp"
#include "../messages/renderables_changed_message.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../components/renderable.hpp"
#include "../components/position.hpp"
#include "../components/building.hpp"
#include "../planet/region.hpp"
#include "../main/game_globals.hpp"
#include "particle_system.hpp"

using namespace rltk;

boost::container::flat_map<int, rltk::vchar> renderables;

void renderables_system::configure() {
    system_name = "Renderables System";
    subscribe_mbox<renderables_changed_message>();
}

void renderables_system::update(const double time_elapsed) {
    std::queue<renderables_changed_message> * render_change = mbox<renderables_changed_message>();
	while (!render_change->empty()) {
		renderables_changed = true;
		render_change->pop();
	}

    if (renderables_changed) {
		renderables.clear();

		// Add buildings to renderables
		each<building_t, position_t>([] (entity_t &entity, building_t &b, position_t &pos) {
			int glyph_idx = 0;
			int offset_x = 0;
			int offset_y = 0;
			if (b.width == 3) offset_x = -1;
			if (b.height == 3) offset_y = -1;

			for (int y = 0; y<b.height; ++y) {
				for (int x=0; x<b.width; ++x) {
					const int idx = mapidx(pos.x+x+offset_x, pos.y+y+offset_y, pos.z);
					renderables[idx] = b.glyphs[glyph_idx];
					if (!b.complete) renderables[idx].foreground = rltk::colors::GREY;
					++glyph_idx;
				}
			}
		});

		// Add other entities
		each<renderable_t, position_t>([] (entity_t &entity, renderable_t &render, position_t &pos) {
			renderables[mapidx(pos.x, pos.y, pos.z)] = rltk::vchar{render.glyph, render.foreground, render.background};
		});

		// Add particles
		for (const particle_t &p : particles) {
			const int idx = mapidx(p.x, p.y, p.z);
			renderables[idx] = p.glyph;
		}

		// If we're rogue mode, and have a path, display it
		if (game_master_mode == ROGUE) {
			entity_t * settler = entity(selected_settler);
			settler_ai_t * ai = settler->component<settler_ai_t>();
			if (ai->current_path && ai->current_path->success) {
				const float n_steps = ai->current_path->steps.size();
				float i = 0;
				for (auto step : ai->current_path->steps) {
					const float lerp_amount = i / n_steps;
					const int idx = mapidx(step.x, step.y, step.z);
					renderables[idx] = rltk::vchar{ 177, lerp(rltk::colors::DARK_GREEN, rltk::colors::LIGHTEST_GREEN, lerp_amount), rltk::colors::BLACK };
					++i;
				}
			}
		}
		
        emit(map_rerender_message{});
	}
}