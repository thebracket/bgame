#include "renderables_system.hpp"
#include "../messages/renderables_changed_message.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../components/renderable.hpp"
#include "../components/position.hpp"
#include "../components/building.hpp"
#include "../planet/region.hpp"

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
		each<renderable_t, position_t>([] (entity_t &entity, renderable_t &render, position_t &pos) {
			renderables[mapidx(pos.x, pos.y, pos.z)] = rltk::vchar{render.glyph, render.foreground, render.background};
		});
		each<building_t, position_t>([] (entity_t &entity, building_t &b, position_t &pos) {
			int glyph_idx = 0;
			for (int y = 0; y<b.height; ++y) {
				for (int x=0; x<b.width; ++x) {
					const int idx = mapidx(pos.x+x, pos.y+y, pos.z);
					renderables[idx] = b.glyphs[glyph_idx];
					if (!b.complete) renderables[idx].foreground = rltk::colors::GREY;
					++glyph_idx;
				}
			}
		});
        emit(map_rerender_message{});
	}
}