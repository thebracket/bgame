#include "blight_builder.hpp"
#include "../../../raws/plants.hpp"
#include "../../../raws/materials.hpp"
#include "../../region/region.hpp"
#include "../../../raws/defs/material_def_t.hpp"
#include "../../../bengine/geometry.hpp"
#include "../../constants.hpp"
#include "../../region/region.hpp"

using namespace region;

void build_ant_mound(bengine::random_number_generator &rng, std::vector<std::tuple<int,int,int>> &spawn_points) noexcept {
    // Build nests and move the creatures into them
    auto blight_mat = get_material_by_tag("blight");
    const int mound_height = rng.roll_dice(3,6);
    const int mound_depth = rng.roll_dice(3,6);

    int x = rng.roll_dice(1,REGION_WIDTH-40)+20;
    int y = rng.roll_dice(1,REGION_HEIGHT-40)+20;
    int z = ground_z(x, y) - mound_depth;
    bool not_in_middle = false;
    while (!not_in_middle) {
        if (bengine::distance2d(x,y, REGION_WIDTH/2, REGION_HEIGHT/2)>10.0F || water_level(mapidx(x,y,z))>0) {
            not_in_middle = true;
            x = rng.roll_dice(1,REGION_WIDTH-10)+5;
            y = rng.roll_dice(1,REGION_HEIGHT-10)+5;
            z = ground_z(x, y) - 20;
            if (z < 10) z = 10;
        }
    }

    // Caves
    const int gz = ground_z(x,y);
    int i = mound_depth;
    for (int sz = z; sz<gz-1; ++sz) {
        for (int X = x-i; X<x+i; ++X) {
            for (int Y = y-i; Y<y+i; ++Y) {
                if (bengine::distance2d(X,Y,x,y) < (i+2.0f)/2.0f) {
                    set_tile_type(mapidx(X, Y, sz), tile_type::FLOOR);
                    spawn_points.emplace_back(std::make_tuple(X,Y,sz));
                }
            }
        }
        --i;
    }

    // Nest: central stair
    for (int sz = z; sz < gz+mound_height; ++sz ) {
        set_tile_type(mapidx(x,y,sz), tile_type::STAIRS_UPDOWN);
    }
    set_tile_type(mapidx(x,y,gz+mound_height), tile_type::STAIRS_DOWN);
    set_tile_type(mapidx(x,y,z), tile_type::STAIRS_UP);

    // Mound above
    i = mound_height;
    for (int sz = gz; sz<gz+mound_height; ++sz) {
        for (int X = x-i; X<x+i; ++X) {
            for (int Y = y-i; Y<y+i; ++Y) {
                if (bengine::distance2d(X,Y,x,y)+1.0f < i/2.0f && sz<REGION_DEPTH && x>0 && x<REGION_WIDTH && y>0 && y<REGION_HEIGHT) {
                    if (sz != gz) {
                        set_tile_type(mapidx(X, Y, sz), tile_type::SOLID);
                        set_tile_material(mapidx(X, Y, sz), blight_mat);
                    }
                    else if (x != X) {
                        set_tile_type(mapidx(X, Y, sz), tile_type::SOLID);
                        set_tile_material(mapidx(X, Y, sz), blight_mat);
                    }
                }
            }
        }
        --i;
    }
}

void just_add_blight(bengine::random_number_generator &rng) noexcept {
    auto red_mushroom = get_plant_idx("deathcap");
    auto green_mushroom = get_plant_idx("blightcap");
    auto blight_mat = get_material_by_tag("blight");

    for_all_tiles([&rng, &red_mushroom, &green_mushroom, &blight_mat] (int idx) {
        auto v = veg_type(idx);
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

        auto m = material(idx);
        if (m > 0) {
            auto mat = get_material(m);
            if (mat->spawn_type == SOIL || mat->spawn_type == SAND) m = blight_mat;
        }
    });
}