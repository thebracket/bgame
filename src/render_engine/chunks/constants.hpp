#pragma once

#include "../../planet/constants.hpp"

namespace chunks {
    constexpr int CHUNK_SIZE = 64;
    constexpr int CHUNK_WIDTH = REGION_WIDTH / CHUNK_SIZE;
    constexpr int CHUNK_HEIGHT = REGION_HEIGHT / CHUNK_SIZE;
    constexpr int CHUNK_DEPTH = REGION_DEPTH / CHUNK_SIZE;
    constexpr int CHUNK_TOTAL = CHUNK_HEIGHT * CHUNK_WIDTH;
    constexpr int CHUNKS_TOTAL = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH;

    constexpr int chunk_idx(const int chunk_x, const int &chunk_y, const int &chunk_z) noexcept {
        return (chunk_z * CHUNK_HEIGHT * CHUNK_WIDTH) + (chunk_y * CHUNK_WIDTH) + chunk_x;
    }

	constexpr int chunk_id_by_world_pos(const int x, const int y, const int z) noexcept
    {
		const int chunk_x = x / CHUNK_SIZE;
		const int chunk_y = y / CHUNK_SIZE;
		const int chunk_z = z / CHUNK_SIZE;
		return chunk_idx(chunk_x, chunk_y, chunk_z);
    }
}