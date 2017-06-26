#pragma once

#include "../../../planet/constants.hpp"

namespace gl {
    constexpr int CHUNK_SIZE = 16;
    constexpr int CHUNKS_WIDTH = REGION_WIDTH / CHUNK_SIZE;
    constexpr int CHUNKS_HEIGHT = REGION_HEIGHT / CHUNK_SIZE;
    constexpr int CHUNKS_DEPTH = REGION_DEPTH / CHUNK_SIZE;
    constexpr int CHUNKS_TOTAL = CHUNKS_DEPTH * CHUNKS_HEIGHT * CHUNKS_WIDTH;

    constexpr int chunk_idx(const int x, const int y, const int z) {
        return (z * CHUNKS_HEIGHT * CHUNKS_WIDTH) + (y * CHUNKS_WIDTH) + x;
    }
}