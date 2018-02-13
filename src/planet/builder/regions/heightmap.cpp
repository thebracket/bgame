#include "heightmap.hpp"
#include "../noise_helper.hpp"
#include "../../planet_builder.hpp"
#include "../../region/region.hpp"
#include "../../../bengine/geometry.hpp"

using namespace region;

std::vector<uint8_t> create_empty_heightmap() noexcept {
    std::vector<uint8_t> heightmap;
    heightmap.resize(REGION_HEIGHT * REGION_WIDTH);
    std::fill(heightmap.begin(), heightmap.end(), (uint8_t)0);
    return heightmap;
}

void build_heightmap_from_noise(std::pair<int,int> &target, FastNoise &noise, std::vector<uint8_t> &heightmap, planet_t &planet) noexcept {
    for (auto y=0; y<REGION_HEIGHT; ++y) {
        for (auto x=0; x<REGION_WIDTH; ++x) {
            const auto nx = noise_x(target.first, x);
            const auto ny = noise_y(target.second, y);
            const auto nh = noise.GetNoise ( nx, ny );
            const auto altitude = noise_to_planet_height(nh);
            const auto cell_idx = (y * REGION_WIDTH) + x;
            heightmap[cell_idx] = altitude - planet.water_height + 5;
        }
    }
}

std::vector<int> create_subregions(planet_t &planet, std::vector<uint8_t> &heightmap, std::pair<biome_t, biome_type_t> &biome, bengine::random_number_generator &rng, std::vector<uint8_t> &pooled_water, std::vector<std::pair<int, uint8_t>> &water_spawners) noexcept
{
    const int region_variance = planet.landblocks[planet.idx(region_x(), region_y())].variance;
    const int n_subregions = 64 + rng.roll_dice(1,20) + (region_variance * 4);

    set_worldgen_status("Finding sub-biomes");
    std::vector<std::pair<int,int>> centroids(n_subregions);
    for (std::size_t i=0; i<n_subregions; ++i) {
        centroids[i] = std::make_pair( rng.roll_dice(1, REGION_WIDTH)-1, rng.roll_dice(1, REGION_HEIGHT)-1 );
    }
    std::vector<int> subregion_idx(REGION_HEIGHT * REGION_WIDTH);

    for (int y=0; y<REGION_HEIGHT; ++y) {
        for (int x=0; x<REGION_WIDTH; ++x) {
            float distance = 20000.0;
            int sub_idx = -1;
            int i=0;
            for (const std::pair<int,int> &centroid : centroids) {
                const float D = bengine::distance2d_squared(x, y, centroid.first, centroid.second);
                if (D < distance) {
                    distance = D;
                    sub_idx = i;
                }
                ++i;
            }
            subregion_idx[(y*REGION_WIDTH) + x] = sub_idx;

        }
    }

    set_worldgen_status("Making sub-biomes");
    std::vector<int> variance;
    for (int i=0; i<n_subregions; ++i) {
        const int up_variance = rng.roll_dice(1, 2)-1;
        const int down_variance = rng.roll_dice(1, 2)-1;
        int amount = up_variance - down_variance;
        // Disable murky pools for now
        //if (rng.roll_dice(1,100000) < rainfall) amount = -10;
        variance.push_back( amount );
    }

    set_worldgen_status("Applying sub-biomes");
    // TODO: Actual variance rather than just altitude when the raws permit it
    for (int y=0; y<REGION_HEIGHT; ++y) {
        for (int x=0; x<REGION_WIDTH; ++x) {
            const int tile_idx = (y * REGION_WIDTH) + x;
            const int sub_idx = subregion_idx[tile_idx];
            const int delta_z = variance[sub_idx];
            if (bengine::distance2d(x,y,REGION_WIDTH/2,REGION_HEIGHT/2) > 20) {
                if (delta_z == -10) {
                    // Murky Pool
                    pooled_water[tile_idx] = 10;
                    heightmap[tile_idx] -= 2;
                    water_spawners.push_back({1, tile_idx}); // Murky pool spawner
                } else {
                    heightmap[tile_idx] += delta_z;
                }
            } else {
                heightmap[tile_idx] = heightmap[ ( REGION_HEIGHT/2 * REGION_WIDTH) + ((REGION_WIDTH/2) - 20) ];
                if (heightmap[tile_idx] < 7) heightmap[tile_idx] = 7;
            }
        }
    }
	return subregion_idx;
}