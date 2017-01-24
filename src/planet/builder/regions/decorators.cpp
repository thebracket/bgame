#include "decorators.hpp"
#include "../../../raws/materials.hpp"
#include "../../planet_builder.hpp"

void build_ramps(region_t &region) {
    for (int y=1; y<REGION_HEIGHT-1; ++y) {
        for (int x=1; x<REGION_WIDTH-1; ++x) {
            const int z = get_ground_z(region,x,y);
            if (region.tile_type[mapidx(x,y,z)] == tile_type::FLOOR) {
                bool is_ramp = false;
                if (region.tile_type[mapidx(x,y-1,z+1)] == tile_type::FLOOR) is_ramp = true;
                if (region.tile_type[mapidx(x,y+1,z+1)] == tile_type::FLOOR) is_ramp = true;
                if (region.tile_type[mapidx(x-1,y,z+1)] == tile_type::FLOOR) is_ramp = true;
                if (region.tile_type[mapidx(x+1,y,z+1)] == tile_type::FLOOR) is_ramp = true;

                if (is_ramp) {
                    const auto idx = mapidx(x,y,z);
                    region.tile_type[idx]=tile_type::RAMP;
                    region.revealed[idx] = true;
                }
            }
        }
    }

    for (int z=1; z<REGION_DEPTH-1; ++z) {
        for (int y=1; y<REGION_HEIGHT-1; ++y) {
            for (int x=1; x<REGION_WIDTH-1; ++x) {
                if (region.tile_type[mapidx(x,y,z)] == tile_type::SOLID) {
                    bool is_edge = false;
                    if (region.tile_type[mapidx(x,y-1,z)] == tile_type::FLOOR || region.tile_type[mapidx(x,y-1,z)] == tile_type::OPEN_SPACE ||region.tile_type[mapidx(x,y-1,z)] == tile_type::RAMP) is_edge = true;
                    if (region.tile_type[mapidx(x,y+1,z)] == tile_type::FLOOR || region.tile_type[mapidx(x,y+1,z)] == tile_type::OPEN_SPACE || region.tile_type[mapidx(x,y+1,z)] == tile_type::RAMP) is_edge = true;
                    if (region.tile_type[mapidx(x-1,y,z)] == tile_type::FLOOR || region.tile_type[mapidx(x-1,y,z)] == tile_type::OPEN_SPACE || region.tile_type[mapidx(x-1,y,z)] == tile_type::RAMP) is_edge = true;
                    if (region.tile_type[mapidx(x+1,y,z)] == tile_type::FLOOR || region.tile_type[mapidx(x+1,y,z)] == tile_type::OPEN_SPACE || region.tile_type[mapidx(x+1,y,z)] == tile_type::RAMP) is_edge = true;

                    if (is_edge) {
                        region.revealed[mapidx(x,y,z)] = true;
                    }
                }
            }
        }
    }
}

void build_debris_trail(region_t &region, const int crash_x, const int crash_y) {
    set_worldgen_status("Crashing the ship");
    for (int x=crash_x - (REGION_WIDTH/4); x<crash_x; ++x) {
		for (int y=crash_y - 3; y<crash_y+4; ++y) {
			int z = get_ground_z(region, x, y);
			region.tile_vegetation_type[mapidx(x,y,z)] = 0;
		}
	}
}

void build_beaches(region_t &region) {
    const auto sand = get_material_by_tag("yellow_sand");
    for (int y=1; y<WORLD_HEIGHT-1; ++y) {
        for (int x=1; x<WORLD_WIDTH; ++x) {
            const int z = get_ground_z(region,x,y);
            if (region.tile_type[mapidx(x,y,z)] == tile_type::FLOOR && region.water_level[mapidx(x,y,z)]==0) {
                bool is_beach = false;
                if (region.water_level[mapidx(x,y-1,z-1)] > 0) is_beach = true;
                if (region.water_level[mapidx(x,y+1,z-1)] > 0) is_beach = true;
                if (region.water_level[mapidx(x-1,y,z-1)] > 0) is_beach = true;
                if (region.water_level[mapidx(x+1,y,z-1)] > 0) is_beach = true;

                if (is_beach) {
                    region.tile_material[mapidx(x,y,z)] = sand;
                    region.tile_vegetation_type[mapidx(x,y,z)] = 0;
                }
            }
        }
    }
}
