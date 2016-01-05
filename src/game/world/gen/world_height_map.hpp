#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <algorithm>
#include <numeric>
#include <iostream>
#include "../world_defs.hpp"

using heightmap_t = std::vector< int >;

/*
 * Creates a new height map
 */
inline std::unique_ptr<heightmap_t> get_height_map()
{
	std::unique_ptr<heightmap_t> result = std::make_unique<heightmap_t>();
	result->resize(NUMBER_OF_TILES_IN_THE_WORLD);
	return std::move(result);
}

/*
 * Deep-copies a height-map, and returns the new copy.
 */
inline std::unique_ptr<heightmap_t> copy_height_map(heightmap_t * original)
{
	std::unique_ptr<heightmap_t> result = std::make_unique<heightmap_t>();
	std::copy(original->begin(), original->end(), std::back_inserter(*result));
	return std::move(result);
}

/*
 * Quick calculation for height-map indices.
 */
inline int height_map_idx(const int x, const int y)
{
	return (y * WORLD_WIDTH * REGION_WIDTH) + x;
}

/*
 * Fill an entire height-map with a specified value.
 */
inline void fill_height_map(heightmap_t * height_map, const uint16_t value)
{
	std::fill(height_map->begin(), height_map->end(), value);
}

/*
 * Fill a height-map with zeroes.
 */
inline void zero_height_map(heightmap_t * height_map)
{
	fill_height_map(height_map, 0);
}

/*
 * Calculate the average height of a height-map.
 */
inline int average_heightmap_height(heightmap_t * height_map)
{
	int sum = std::accumulate(height_map->begin(), height_map->end(), 0);
	return sum / height_map->size();
}

/*
 * Calculates the lowest height-map height.
 */
inline int min_heightmap_height(heightmap_t * height_map)
{
	return *std::min_element(height_map->begin(), height_map->end());
}

/*
 * Calculates the highest height-map height.
 */
inline int max_heightmap_height(heightmap_t * height_map)
{
	return *std::max_element(height_map->begin(), height_map->end());
}

/*
 * Smooth a height-map by averaging each cell with its neighbors. Utilizes
 * a copy, to avoid artifacts.
 */
inline void smooth_height_map(heightmap_t * height_map)
{
	std::unique_ptr<heightmap_t> tmp_p = get_height_map();

	for (int y = 1; y < (WORLD_HEIGHT * REGION_HEIGHT) - 1; ++y)
	{
		for (int x = 1; x < (WORLD_WIDTH * REGION_WIDTH) - 1; ++x)
		{
			const int sum = height_map->operator[](height_map_idx(x - 1, y - 1))
					+ height_map->operator[](height_map_idx(x, y - 1))
					+ height_map->operator[](height_map_idx(x + 1, y - 1))
					+ height_map->operator[](height_map_idx(x - 1, y))
					+ height_map->operator[](height_map_idx(x, y))
					+ height_map->operator[](height_map_idx(x + 1, y))
					+ height_map->operator[](height_map_idx(x - 1, y + 1))
					+ height_map->operator[](height_map_idx(x, y + 1))
					+ height_map->operator[](height_map_idx(x + 1, y + 1));
			const uint16_t average = sum / 9;
			tmp_p->operator[](height_map_idx(x, y)) = average;
		}
	}

	height_map->clear();
	std::copy(tmp_p->begin(), tmp_p->end(), std::back_inserter(*height_map));
}

/*
 * Determines the altitude at which flooding occurs, aiming for desired_percentage percent
 * of all tiles in the world to be under water.
 */
inline int find_flood_level(heightmap_t * height_map, float desired_percentage)
{
	const int n_tiles_required = (desired_percentage
			* NUMBER_OF_TILES_IN_THE_WORLD);
	bool found = false;
	int level = 1;
	int running_total = 0;
	while (!found)
	{
		for (int y = 1; y < (WORLD_HEIGHT * REGION_HEIGHT) - 1; ++y)
		{
			for (int x = 1; x < (WORLD_WIDTH * REGION_WIDTH) - 1; ++x)
			{
				const int widx = height_map_idx(x, y);
				if (height_map->operator[](widx) < level)
					++running_total;
			}
		}
		if (running_total >= n_tiles_required)
		{
			found = true;
		}
		else
		{
			++level;
		}
	}
	std::cout << "Flood level: " << level << "\n";
	return level;
}
