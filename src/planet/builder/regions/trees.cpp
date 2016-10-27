#include "trees.hpp"

using namespace rltk;

inline void set_tree_trunk(region_t &region, const int x, const int y, const int z, const int tree_id) {
	if (x>0 && y>0 && z>0 && x<REGION_WIDTH-1 && y<REGION_HEIGHT-1 && z<REGION_DEPTH-2) {
		const auto idx = mapidx(x,y,z);
        region.tile_type[idx] = tile_type::TREE_TRUNK;
        region.tree_id[idx] = tree_id;
        region.solid[idx] = true;
	}
}

inline void set_tree_foliage(region_t &region, const int x, const int y, const int z, const int tree_id) {
	if (x>0 && y>0 && z>0 && x<REGION_WIDTH-1 && y<REGION_HEIGHT-1 && z<REGION_DEPTH-2) {
		const int idx = mapidx(x,y,z);
        region.tile_type[idx] = tile_type::TREE_LEAF;
        region.tree_id[idx] = tree_id;
        region.solid[idx] = true;
	}
}

inline void plant_tree_deciduous(region_t &region, const int x, const int y, const int z, random_number_generator &rng) {
	// Trees get their own entity
	const int tree_height = 1 + rng.roll_dice(2,4);
	for (int i=0; i<tree_height; ++i) {
		set_tree_trunk(region, x, y, z+i, region.next_tree_id);
		if ( i > tree_height/2) {
			const int radius = (tree_height - i) + 1;
			for (int X=x-radius; X<x+radius; ++X) {
				for (int Y=y-radius; Y<y+radius; ++Y) {
					const float distance = distance2d(x,y,X,Y);
					if (distance <= radius && (X!=x || Y!=y)) {
						set_tree_foliage(region, X, Y, z+i, region.next_tree_id);
					}
				}
			}
		}
	}
	++region.next_tree_id;
}

inline void plant_tree_evergreen(region_t &region, const int x, const int y, const int z, random_number_generator &rng) {
	// Trees get their own entity
	const int tree_height = 1 + rng.roll_dice(2,3);
	for (int i=0; i<tree_height; ++i) {
		set_tree_trunk(region, x, y, z+i, region.next_tree_id);
		if ( i > 0) {
			const int radius = (tree_height - i)/2 + 1;
			for (int X=x-radius; X<x+radius; ++X) {
				for (int Y=y-radius; Y<y+radius; ++Y) {
					const float distance = distance2d(x,y,X,Y);
					if (distance <= radius && (X!=x || Y!=y)) {
						set_tree_foliage(region, X, Y, z+i, region.next_tree_id);
					}
				}
			}
		}
	}
	++region.next_tree_id;
}

void build_trees(region_t &region, std::pair<biome_t, biome_type_t> &biome, random_number_generator &rng) {
    const int d_chance = biome.second.deciduous_tree_chance;
    const int e_chance = biome.second.evergreen_tree_chance;

    for (int y=10; y<REGION_HEIGHT-10; ++y) {
        for (int x=10; x<REGION_WIDTH-10; ++x) {
            const int z = get_ground_z(region,x,y);
            const auto idx = mapidx(x,y,z);
            const int crash_distance = distance2d(x,y,REGION_WIDTH/2,REGION_HEIGHT/2);
            if (crash_distance > 20 && region.tile_type[idx] == tile_type::FLOOR && region.water_level[idx]==0) {
                int dice_roll = rng.roll_dice(1,1000);
                if (dice_roll <= d_chance) {
                    // Grow a decidous tree
                    plant_tree_deciduous(region, x, y, z, rng);
                } else {
                    dice_roll = rng.roll_dice(1,1000);
                    if (dice_roll < e_chance) {
                        plant_tree_evergreen(region, x, y, z, rng);
                    }
                }
            }
        }
    }
}
