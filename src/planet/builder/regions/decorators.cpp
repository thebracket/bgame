#include "decorators.hpp"
#include "../../../raws/materials.hpp"
#include "../../planet_builder.hpp"
#include "../../region/region.hpp"
#include "../../../raws/raws.hpp"

using namespace region;

void build_ramps(const std::vector<uint8_t> pooled_water) noexcept {
    for (int y=1; y<REGION_HEIGHT-1; ++y) {
        for (int x=1; x<REGION_WIDTH-1; ++x) {
            const int z = ground_z(x,y);
            if (region::tile_type(mapidx(x,y,z)) == tile_type::FLOOR && pooled_water[(y*REGION_WIDTH)+x]==0) {
                bool is_ramp = false;
                if (region::tile_type(mapidx(x,y-1,z+1)) == tile_type::FLOOR) is_ramp = true;
                if (region::tile_type(mapidx(x,y+1,z+1)) == tile_type::FLOOR) is_ramp = true;
                if (region::tile_type(mapidx(x-1,y,z+1)) == tile_type::FLOOR) is_ramp = true;
                if (region::tile_type(mapidx(x+1,y,z+1)) == tile_type::FLOOR) is_ramp = true;

                if (is_ramp) {
                    const auto idx = mapidx(x,y,z);
                    set_tile_type(idx, tile_type::RAMP);
                    reveal(idx);
                }
            }
        }
    }

    for (int z=1; z<REGION_DEPTH-1; ++z) {
        for (int y=1; y<REGION_HEIGHT-1; ++y) {
            for (int x=1; x<REGION_WIDTH-1; ++x) {
                if (region::tile_type(mapidx(x,y,z)) == tile_type::SOLID) {
                    bool is_edge = false;
                    if (region::tile_type(mapidx(x,y-1,z)) == tile_type::FLOOR || region::tile_type(mapidx(x,y-1,z)) == tile_type::OPEN_SPACE || region::tile_type(mapidx(x,y-1,z)) == tile_type::RAMP) is_edge = true;
                    if (region::tile_type(mapidx(x,y+1,z)) == tile_type::FLOOR || region::tile_type(mapidx(x,y+1,z)) == tile_type::OPEN_SPACE || region::tile_type(mapidx(x,y+1,z)) == tile_type::RAMP) is_edge = true;
                    if (region::tile_type(mapidx(x-1,y,z)) == tile_type::FLOOR || region::tile_type(mapidx(x-1,y,z)) == tile_type::OPEN_SPACE || region::tile_type(mapidx(x-1,y,z)) == tile_type::RAMP) is_edge = true;
                    if (region::tile_type(mapidx(x+1,y,z)) == tile_type::FLOOR || region::tile_type(mapidx(x+1,y,z)) == tile_type::OPEN_SPACE || region::tile_type(mapidx(x+1,y,z)) == tile_type::RAMP) is_edge = true;

                    if (is_edge) {
                        reveal(mapidx(x,y,z));
                    }
                }
            }
        }
    }
}

void build_debris_trail(const int crash_x, const int crash_y) noexcept {
    set_worldgen_status("Crashing the ship");
    for (auto x=crash_x - (REGION_WIDTH/4); x<crash_x; ++x) {
		for (auto y=crash_y - 3; y<crash_y+4; ++y) {
			auto z = ground_z(x, y);
			const auto idx = mapidx(x, y, z);
			set_veg_type(idx, 0);
			if (tree_id(idx) > 0) {
				// Tree needs destroying
				const auto target_tree = tree_id(idx);

				auto number_of_logs = 0;
				auto tree_idx = 0;
				auto lowest_z = 1000;

				for (auto Z = 0; Z < REGION_DEPTH; ++Z) {
					for (auto Y = 0; Y < REGION_HEIGHT; ++Y) {
						for (auto X = 0; X < REGION_WIDTH; ++X) {
							const auto tidx = mapidx(X, Y, Z);
							if (tree_id(tidx) == target_tree) {
								if (Z < lowest_z) {
									lowest_z = Z;
									tree_idx = tidx;
								}

								set_tile_type(tidx, tile_type::OPEN_SPACE);
								set_tree_id(tidx, 0);
								++number_of_logs;
							}
						}
					}
				}

				set_tile_type(idx, tile_type::FLOOR);
				// Spawn wooden logs
				number_of_logs = (number_of_logs / 20) + 1;
				for (auto i = 0; i<number_of_logs; ++i) {
					const std::string cname = "Crashing Ship";
					spawn_item_on_ground(x, y, z, "wood_log", get_material_by_tag("wood"), 3, 100, 0, cname);
				}
			}

		}
	}
}

void build_beaches() noexcept {
    const auto sand = get_material_by_tag("yellow_sand");
    for (int y=1; y<WORLD_HEIGHT-1; ++y) {
        for (int x=1; x<WORLD_WIDTH; ++x) {
            const int z = ground_z(x,y);
            if (region::tile_type(mapidx(x,y,z)) == tile_type::FLOOR && water_level(mapidx(x,y,z))==0) {
                bool is_beach = false;
                if (water_level(mapidx(x,y-1,z-1)) > 0) is_beach = true;
                if (water_level(mapidx(x,y+1,z-1)) > 0) is_beach = true;
                if (water_level(mapidx(x-1,y,z-1)) > 0) is_beach = true;
                if (water_level(mapidx(x+1,y,z-1)) > 0) is_beach = true;

                if (is_beach) {
                    set_tile_material(mapidx(x,y,z), sand);
                    set_veg_type(mapidx(x,y,z), 0);
                }
            }
        }
    }
}
