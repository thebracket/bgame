#pragma once

#include <vector>
#include <string>

namespace gl {
    constexpr int num_model_items = 8;
    struct model_t {
        std::vector<float> items;
        /* Layout:
         * 0,1,2    Vertex Position
         * 3,4,5    Normal Position
         * 6,7      Texture UV
         */
        unsigned int vbo_id;
        int texture_id;
    };

    void setup_model(const int &idx, const std::string &filename, const int &texture_id);
    model_t * get_model(const int &idx);
}