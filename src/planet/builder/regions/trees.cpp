#include "trees.hpp"
#include "../../region/region.hpp"
#include "../../../bengine/geometry.hpp"

using namespace region;

inline void set_tree_trunk(const int x, const int y, const int z, const int tree_id) {
	if (x>0 && y>0 && z>0 && x<REGION_WIDTH-1 && y<REGION_HEIGHT-1 && z<REGION_DEPTH-2) {
		const auto idx = mapidx(x,y,z);
        set_tile_type(idx, tile_type::TREE_TRUNK);
        set_tree_id(idx, tree_id);
        set_solid(idx, true);
	}
}

inline void set_tree_foliage(const int x, const int y, const int z, const int tree_id) {
	if (x>0 && y>0 && z>0 && x<REGION_WIDTH-1 && y<REGION_HEIGHT-1 && z<REGION_DEPTH-2) {
		const int idx = mapidx(x,y,z);
        set_tile_type(idx, tile_type::TREE_LEAF);
        set_tree_id(idx, tree_id);
        set_solid(idx, true);
	}
}

inline void plant_tree_deciduous(const int x, const int y, const int z, bengine::random_number_generator &rng) {
	// Trees get their own entity
	const int tree_height = 1 + rng.roll_dice(2,4);
	for (int i=0; i<tree_height; ++i) {
		set_tree_trunk(x, y, z+i, next_tree_id());
		if ( i > tree_height/2) {
			const int radius = (tree_height - i) + 1;
			for (int X=x-radius; X<x+radius; ++X) {
				for (int Y=y-radius; Y<y+radius; ++Y) {
					const float distance = bengine::distance2d(x,y,X,Y);
					if (distance <= radius && (X!=x || Y!=y)) {
						set_tree_foliage(X, Y, z+i, next_tree_id());
					}
				}
			}
		}
	}
	inc_next_tree();
}

inline void plant_tree_evergreen(const int x, const int y, const int z, bengine::random_number_generator &rng) {
	// Trees get their own entity
	const int tree_height = 1 + rng.roll_dice(2,3);
	for (int i=0; i<tree_height; ++i) {
		set_tree_trunk(x, y, z+i, next_tree_id());
		if ( i > 0) {
			const int radius = (tree_height - i)/2 + 1;
			for (int X=x-radius; X<x+radius; ++X) {
				for (int Y=y-radius; Y<y+radius; ++Y) {
					const float distance = bengine::distance2d(x,y,X,Y);
					if (distance <= radius && (X!=x || Y!=y)) {
						set_tree_foliage(X, Y, z+i, next_tree_id());
					}
				}
			}
		}
	}
	inc_next_tree();
}

bool can_see_sky(const int &x, const int &y, const int &z) {
	bool result = true;

	int Z = z;
	while (Z < REGION_DEPTH) {
		if (solid(mapidx(x,y,Z))) result = false;
		Z++;
	}
	return result;
}

void build_trees(std::pair<biome_t, biome_type_t> &biome, bengine::random_number_generator &rng) {
    const int d_chance = biome.second.deciduous_tree_chance;
    const int e_chance = biome.second.evergreen_tree_chance;

    for (int y=10; y<REGION_HEIGHT-10; ++y) {
        for (int x=10; x<REGION_WIDTH-10; ++x) {
            const int z = ground_z(x,y);
            const auto idx = mapidx(x,y,z);
            const int crash_distance = bengine::distance2d(x,y,REGION_WIDTH/2,REGION_HEIGHT/2);
            if (crash_distance > 20 && region::tile_type(idx) == tile_type::FLOOR && water_level(idx)==0 && can_see_sky(x,y,z)) {
                int dice_roll = rng.roll_dice(1,1000);
                if (dice_roll <= d_chance) {
                    // Grow a decidous tree
                    plant_tree_deciduous(x, y, z, rng);
                } else {
                    dice_roll = rng.roll_dice(1,1000);
                    if (dice_roll < e_chance) {
                        plant_tree_evergreen(x, y, z, rng);
                    }
                }
            }
        }
    }
}
