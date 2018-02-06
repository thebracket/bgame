#pragma once

#include <string>
#include <vector>

/*
 * Defines template for clothes. These need to be merged into items eventually.
 */
struct clothing_t {
    std::string name= "";
    std::vector<std::string> colors;
    std::string slot = "";
    std::string description = "";
    float armor_class = 0.0F;
    uint16_t clothing_glyph = 0;
	int clothing_layer = 0;
	int voxel_model = 0;
};