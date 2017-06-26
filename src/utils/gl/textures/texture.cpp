#include "texture.hpp"
#include "stb_image.h"
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif
#include <boost/container/flat_map.hpp>
#include <iostream>

namespace textures {

    boost::container::flat_map<int, texture_t> atlas;

    void texture_t::load() {
        std::cout << "Lazy loading texture index " << filename << "\n";

        int width, height, bpp;
        stbi_set_flip_vertically_on_load(true);

        unsigned char *image_data = stbi_load(filename.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
        if (image_data == nullptr) throw std::runtime_error(std::string("Cannot open: ") + std::string(filename));
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(image_data);

        unsigned char *normal_data = stbi_load(normal_filename.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
        if (normal_data == nullptr) throw std::runtime_error(std::string("Cannot open: ") + std::string(normal_filename));
        glGenTextures(1, &normal_id);
        glBindTexture(GL_TEXTURE_2D, normal_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, normal_data);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(normal_data);

        std::cout << "Bound as IDs: " << texture_id << "/" << normal_id << "\n";
        loaded = true;
    }

    void texture_t::unload() {
        glDeleteTextures(1, &texture_id);
        glDeleteTextures(1, &normal_id);
        loaded = false;
    }

    void load_textures(std::vector<std::tuple<int, std::string, std::string>> textures) {
        for (const auto &tex : textures) {
            atlas[std::get<0>(tex)] = texture_t(std::get<1>(tex).c_str(), std::get<2>(tex).c_str());
        }
    }

    texture_t * get_texture_by_id(const int &id) {
        auto finder = atlas.find(id);
        if (finder == atlas.end()) {
            return &atlas[1];
        }
        if (!finder->second.loaded) {
            finder->second.load();
        }
        ++finder->second.usage_counter;
        return &finder->second;
    }

    void texture_start() {
        for (auto it=atlas.begin(); it!=atlas.end(); ++it) {
            it->second.usage_counter = 0;
        }
    }

    void texture_cleanup() {
        for (auto it=atlas.begin(); it!=atlas.end(); ++it) {
            if (it->second.usage_counter == 0) it->second.unload();
        }
    }

}