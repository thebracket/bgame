#pragma once

#include <vector>
#include <string>

namespace textures {

    struct texture_t {
        texture_t() {}

        texture_t(const char *filename, const char *normal_filename);

        unsigned int texture_id;
        unsigned int normal_id;
    };

    void load_textures(std::vector<std::tuple<int, std::string, std::string>> textures);
    texture_t * get_texture_by_id(const int &id);
}