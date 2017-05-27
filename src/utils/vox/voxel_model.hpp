#pragma once

#include <vector>

struct subvoxel {
    float x, y, z;
    float r, g, b;
};

struct voxel_model {
    int width, height, depth;
    std::vector<subvoxel> voxels;
};