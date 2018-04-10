#include "stdafx.h"
#include "visibility_system.hpp"
#include "../../planet/region/region.hpp"
#include "../../bengine/geometry.hpp"
#include "../../global_assets/spatial_db.hpp"

using namespace bengine;
using namespace tile_flags;

namespace systems {
	namespace visibility {

		std::unordered_set<std::size_t> blocked_visibility;
		std::unordered_set<std::size_t> dirty_entities;
		bool opacity_dirty = true;
		bool dirty = true;

		void opacity_is_dirty() {
			opacity_dirty = true;
		}

		void on_entity_moved(int &entity_id) {
			dirty_entities.insert(entity_id);
			dirty = true;
		}

		static void reveal(const int &idx, viewshed_t &view) {
			if (view.good_guy_visibility) region::reveal(idx);
			view.visible_cache.insert(idx);
		}

		inline void internal_pen_view_to(position_t &pos, viewshed_t &view, int x, int y, int z) {
			const float dist_square = view.viewshed_radius * view.viewshed_radius;

			line_func_3d_cancellable(pos.x, pos.y, pos.z, pos.x + x, pos.y + y, pos.z + z, [&view, &pos, &dist_square](int X, int Y, int Z) {
				reveal(mapidx(X, Y, Z), view);
				const float distance = distance3d_squared(pos.x, pos.y, pos.z, X, Y, Z);
				if (distance < dist_square) {
					return true;
				}
				else {
					return false;
				}
			});
		}

		static void internal_view_to(position_t &pos, viewshed_t &view, const int x, const int y, const int z) {
			using namespace region;

			const float dist_square = view.viewshed_radius * view.viewshed_radius;

			//std::cout << "Starting at " << pos.x << ", " << pos.y << ", " << pos.z << "\n";
			//std::cout << "Ending at " << pos.x+x << ", " << pos.y+y << ", " << pos.z+z << "\n";
			reveal(mapidx(pos), view); // Always reveal where we are standing

			auto blocked = false;
			line_func_3d(pos.x, pos.y, pos.z, pos.x + x, pos.y + y, pos.z + z, [&view, &pos, &dist_square, &blocked] (const int at_x, const int at_y, const int at_z)
			{
				if (at_x > 0 && at_x < REGION_WIDTH - 1 && at_y > 0 && at_y < REGION_HEIGHT - 1 && at_z > 0 && at_z < REGION_DEPTH-1) {
					const auto distance = distance3d_squared(pos.x, pos.y, pos.z, at_x, at_y, at_z);
					if (distance < dist_square) {
						//std::cout << " - Visit " << at_x << ", " << at_y << ", " << at_z << "\n";
						const auto idx = mapidx(at_x, at_y, at_z);
						if (!blocked) reveal(idx, view);
						if (region::flag(idx, SOLID)) blocked = true;
					}
				}
			});
		}

		static void update_penetrating_viewshed(entity_t &e, position_t &pos, viewshed_t &view) {
			view.visible_cache.clear();

			reveal(mapidx(pos.x, pos.y, pos.z), view);
			for (int z = (0 - view.viewshed_radius); z<view.viewshed_radius; ++z) {
				for (int i = 0 - view.viewshed_radius; i<view.viewshed_radius; ++i) {
					internal_pen_view_to(pos, view, i, 0 - view.viewshed_radius, z);
					internal_pen_view_to(pos, view, i, view.viewshed_radius, z);
					internal_pen_view_to(pos, view, 0 - view.viewshed_radius, i, z);
					internal_pen_view_to(pos, view, view.viewshed_radius, i, z);
				}
			}
		}

		static void update_normal_viewshed(entity_t &e, position_t &pos, viewshed_t &view) {
			view.visible_cache.clear();
			reveal(mapidx(pos.x, pos.y, pos.z), view);
			for (auto z = (0 - view.viewshed_radius); z<view.viewshed_radius; ++z) {
				for (auto i = 0 - view.viewshed_radius; i<view.viewshed_radius; ++i) {
					internal_view_to(pos, view, i, view.viewshed_radius, z);
					internal_view_to(pos, view, i, -view.viewshed_radius, z);
					internal_view_to(pos, view, view.viewshed_radius, i, z);
					internal_view_to(pos, view, -view.viewshed_radius, i, z);
				}
			}
		}

		void calculate_building_opacity() {
			blocked_visibility.clear();
			each<building_t, position_t>([](entity_t &e, building_t &b, position_t &pos) {
				if (!b.complete) return;
				if (b.height == 1 && b.width == 1) {
					blocked_visibility.insert(mapidx(pos.x, pos.y, pos.z));
				}
				else if (b.height == 3 && b.width == 3) {
					blocked_visibility.insert(mapidx(pos.x - 1, pos.y - 1, pos.z));
					blocked_visibility.insert(mapidx(pos.x, pos.y - 1, pos.z));
					blocked_visibility.insert(mapidx(pos.x + 1, pos.y - 1, pos.z));
					blocked_visibility.insert(mapidx(pos.x - 1, pos.y, pos.z));
					blocked_visibility.insert(mapidx(pos.x, pos.y, pos.z));
					blocked_visibility.insert(mapidx(pos.x + 1, pos.y, pos.z));
					blocked_visibility.insert(mapidx(pos.x - 1, pos.y + 1, pos.z));
					blocked_visibility.insert(mapidx(pos.x, pos.y + 1, pos.z));
					blocked_visibility.insert(mapidx(pos.x + 1, pos.y + 1, pos.z));
				}
				else {
					for (int y = pos.y; y<pos.y + b.height; ++y) {
						for (int x = pos.x; x<pos.x + b.width; ++x) {
							blocked_visibility.insert(mapidx(x, y, pos.z));
						}
					}
				}
			});
		}

		void run(const double &duration_ms) {
			using namespace region;

			if (opacity_dirty) {
				calculate_building_opacity();
				opacity_dirty = false;
			}

			if (!dirty) return;

			// Apply to map - first clear everything
			clear_visibility();

			each<position_t, viewshed_t>([](entity_t &e, position_t &pos, viewshed_t &view) {
				// Create viewsheds if needed
				if (view.visible_cache.empty() || dirty_entities.find(e.id) != dirty_entities.end()) {
					// The entity needs a viewshed!
					if (view.penetrating) {
						update_penetrating_viewshed(e, pos, view);
					}
					else {
						update_normal_viewshed(e, pos, view);
					}
					dirty_entities.erase(e.id);
				}

				// Make visible
				if (view.good_guy_visibility) {
					for (const int &idx : view.visible_cache) {
						make_visible(idx);
					}
				}

				// What can we see? - Grazers, Sentients, Turrets and Settlers only
				auto grazer = e.component<grazer_ai>();
				auto settler = e.component<settler_ai_t>();
				auto sentient = e.component<sentient_ai>();
				auto turret = e.component<turret_t>();
				auto sensor = e.component<proximity_sensor_t>();
				if (grazer || settler || sentient || turret || sensor) {
					view.visible_entities.clear();
					for (const int &idx : view.visible_cache) {
						auto [x,y,z] = idxmap(idx);
						auto visible_here = entity_octree.find_by_loc(octree_location_t{ x, y, z, 0 });
						for (const auto &v : visible_here) {
							view.visible_entities.insert(v);
						}
					}
				}
			});
			dirty_entities.clear();
			dirty = false;
		}
	}
}