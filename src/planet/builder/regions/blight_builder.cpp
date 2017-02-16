#include "blight_builder.hpp"
#include "../../../raws/plants.hpp"
#include <algorithm>
#include "../../../components/position.hpp"
#include "../../../components/sentient_ai.hpp"
#include "../../../raws/materials.hpp"

void build_ant_mound(region_t &region, random_number_generator &rng) {
    // Build nests and move the creatures into them
    auto blight_mat = get_material_by_tag("blight");
    const int mound_height = rng.roll_dice(3,6);
    const int mound_depth = rng.roll_dice(3,6);

    int x = rng.roll_dice(1,REGION_WIDTH-10)+5;
    int y = rng.roll_dice(1,REGION_HEIGHT-10)+5;
    int z = get_ground_z(region, x, y) - mound_depth;
    bool not_in_middle = false;
    while (!not_in_middle) {
        if (distance2d(x,y, REGION_WIDTH/2, REGION_HEIGHT/2)>10.0F || region.water_level[mapidx(x,y,z)]>0) {
            not_in_middle = true;
            x = rng.roll_dice(1,REGION_WIDTH-10)+5;
            y = rng.roll_dice(1,REGION_HEIGHT-10)+5;
            z = get_ground_z(region, x, y) - 20;
            if (z < 10) z = 10;
        }
    }

    // Caves
    std::vector<int> spawn_points;
    const int ground_z = get_ground_z(region, x,y);
    int i = mound_depth;
    for (int sz = z; sz<ground_z-1; ++sz) {
        for (int X = x-i; X<x+i; ++X) {
            for (int Y = y-i; Y<y+i; ++Y) {
                if (distance2d(X,Y,x,y)+1.0f < i/2.0f) {
                    region.tile_type[mapidx(X, Y, sz)] = tile_type::FLOOR;
                    spawn_points.push_back(mapidx(X,Y,sz));
                }
            }
        }
        --i;
    }

    // Nest: central stair
    for (int sz = z; sz < ground_z; ++sz ) {
        region.tile_type[mapidx(x,y,sz)] = tile_type::STAIRS_UPDOWN;
    }
    region.tile_type[mapidx(x,y,ground_z)] = tile_type::STAIRS_DOWN;
    region.tile_type[mapidx(x,y,z)] == tile_type::STAIRS_UP;

    // Mound above
    i = mound_height;
    for (int sz = ground_z; sz<ground_z+mound_height; ++sz) {
        for (int X = x-i; X<x+i; ++X) {
            for (int Y = y-i; Y<y+i; ++Y) {
                if (distance2d(X,Y,x,y)+1.0f < i/2.0f && sz<REGION_DEPTH && x>0 && x<REGION_WIDTH && y>0 && y<REGION_HEIGHT) {
                    if (sz != ground_z) {
                        region.tile_type[mapidx(X, Y, sz)] = tile_type::SOLID;
                        region.tile_material[mapidx(X, Y, sz)] = blight_mat;
                    }
                    else if (x != X) {
                        region.tile_type[mapidx(X, Y, sz)] = tile_type::SOLID;
                        region.tile_material[mapidx(X, Y, sz)] = blight_mat;
                    }
                }
            }
        }
        --i;
    }

    i = 0;
    each<sentient_ai, position_t>([&i, &spawn_points, &x, &y, &z] (entity_t &e, sentient_ai &ai, position_t &pos) {
        const std::size_t spawn_idx = i % spawn_points.size();
        std::tie(pos.x, pos.y, pos.z) = idxmap(spawn_points[spawn_idx]);
    });
}

void just_add_blight(region_t &region, random_number_generator &rng) {
    auto red_mushroom = get_plant_idx("deathcap");
    auto green_mushroom = get_plant_idx("blightcap");
    for (auto &v : region.tile_vegetation_type) {
        if (v > 0) {
            const int roll = rng.roll_dice(1,6);
            if (roll <= 2) {
                v = red_mushroom;
            } else if (roll < 6) {
                v = green_mushroom;
            } else {
                v = 0;
            }
        }
    }
    auto blight_mat = get_material_by_tag("blight");
    for (auto &m : region.tile_material) {
        if (m > 0) {
            auto mat = get_material(m);
            if (mat->spawn_type == soil || mat->spawn_type == sand) m = blight_mat;
        }
    }
}