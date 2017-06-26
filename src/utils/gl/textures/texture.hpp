#pragma once

#include <vector>
#include <string>

namespace textures {

    struct texture_t {
        texture_t() {}

        texture_t(const char *fn, const char *normal) : filename(fn), normal_filename(normal) {
        }

        void load();
        void unload();

        std::string filename;
        std::string normal_filename;
        bool loaded = false;
        unsigned int texture_id;
        unsigned int normal_id;
        int usage_counter = 0;
    };

    void load_textures(std::vector<std::tuple<int, std::string, std::string>> textures);
    texture_t * get_texture_by_id(const int &id);
    void texture_start();
    void texture_cleanup();
}