#include "visibility_system.hpp"
#include "../game_globals.hpp"
#include "../messages/messages.hpp"

using namespace rltk;

inline void reveal(const int &idx, viewshed_t &view) {
	current_region.tiles[idx].flags.set(tile_flags::REVEALED);
	view.visible_cache.push_back(idx);
}

inline void internal_pen_view_to(position_t &pos, viewshed_t &view, int x, int y, int z) {
	const float dist_square = view.viewshed_radius * view.viewshed_radius;

	line_func_3d_cancellable(pos.x, pos.y, pos.z, pos.x+x, pos.y+y, pos.z+z, [&view, &pos, &dist_square] (int X, int Y, int Z) {
		reveal(mapidx(X, Y, Z), view);
		const float distance = distance3d_squared(pos.x, pos.y, pos.z, X, Y, Z);
		if (distance < dist_square) {
			return true;
		} else {
			return false;
		}
	});
}

inline void internal_view_to(position_t &pos, viewshed_t &view, int x, int y, int z) {
	const float dist_square = view.viewshed_radius * view.viewshed_radius;

	line_func_3d_cancellable(pos.x, pos.y, pos.z, pos.x+x, pos.y+y, pos.z+z, [&view, &pos, &dist_square] (int X, int Y, int Z) {
		const int idx = mapidx(X, Y, Z);
		reveal(idx, view);
		const float distance = distance3d_squared(pos.x, pos.y, pos.z, X, Y, Z);
		if (distance > dist_square) {
			return false;
		}
		return !(current_region.tiles[idx].flags.test(tile_flags::SOLID));
	});
}

void update_penetrating_viewshed(entity_t &e, position_t &pos, viewshed_t &view) {
	view.visible_cache.clear();

	reveal(mapidx(pos.x, pos.y, pos.z), view);
	for (int z=(0-view.viewshed_radius); z<view.viewshed_radius; ++z) {
		for (int i=0-view.viewshed_radius; i<view.viewshed_radius; ++i) {
			internal_pen_view_to(pos, view, i, 0-view.viewshed_radius, z);
			internal_pen_view_to(pos, view, i, view.viewshed_radius, z);
			internal_pen_view_to(pos, view, 0-view.viewshed_radius, i, z);
			internal_pen_view_to(pos, view, view.viewshed_radius, i, z);
		}
	}
}

void update_normal_viewshed(entity_t &e, position_t &pos, viewshed_t &view) {
	view.visible_cache.clear();
	reveal(mapidx(pos.x, pos.y, pos.z), view);
	for (int z=(0-view.viewshed_radius); z<view.viewshed_radius; ++z) {
		for (int i=0-view.viewshed_radius; i<view.viewshed_radius; ++i) {
			internal_view_to(pos, view, i, 0-view.viewshed_radius, z);
			internal_view_to(pos, view, i, view.viewshed_radius, z);
			internal_view_to(pos, view, 0-view.viewshed_radius, i, z);
			internal_view_to(pos, view, view.viewshed_radius, i, z);
		}
	}
}

void visibility_system::update(const double duration_ms) {
	if (!dirty) return;

	// Apply to map - first clear everything
	for (tile_t &tile : current_region.tiles) {
		tile.flags.reset(tile_flags::VISIBLE);
	}

	each<position_t, viewshed_t>([this] (entity_t &e, position_t &pos, viewshed_t &view) {
		// Create viewsheds if needed
		if (view.visible_cache.empty() || dirty_entities.find(e.id) != dirty_entities.end()) {
			// The entity needs a viewshed!
			if (view.penetrating) {
				update_penetrating_viewshed(e, pos, view);
			} else {
				update_normal_viewshed(e, pos, view);
			}
		}

		// Make visible
		for (const int &idx : view.visible_cache) {
			current_region.tiles[idx].flags.set(tile_flags::VISIBLE);
		}
	});
	dirty_entities.clear();
	dirty = false;
}

void visibility_system::configure() {
	subscribe<entity_moved_message>([this] (entity_moved_message &msg) {
		dirty_entities.insert(msg.entity_id);
		dirty = true;
	});
}
