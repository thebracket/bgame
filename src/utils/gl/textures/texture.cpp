#include "texture.hpp"
#include "stb_image.h"
#include "../gl_include.hpp"
#include <boost/container/flat_map.hpp>
#include <iostream>
#include "stb_image.h"

namespace textures {

    boost::container::flat_map<int, texture_t> atlas;
    int next_texture_id = 0;

    int load_new_texture(const std::string filename)
    {
        int width, height, bpp;
        stbi_set_flip_vertically_on_load(true);

        unsigned char *image_data = stbi_load(filename.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
        if (image_data == nullptr) throw std::runtime_error(std::string("Cannot open: ") + std::string(filename));
        if (width != 128 || height != 128) throw std::runtime_error(std::string("Textures must be 128x128. Failed on: ") + std::string(filename));

        int result_id = next_texture_id;
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, result_id, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        stbi_image_free(image_data);

        ++next_texture_id;
        return result_id;
    }

    void load_textures(std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> textures)
    {
        constexpr int tex_height = 128;
        constexpr int tex_width = 128;
        constexpr int num_mipmaps = 1;
        const int n_textures = textures.size() * 4;

        // Create a texture array to see if it works
        unsigned int tex_array_id = 0;
        glGenTextures(1, &tex_array_id);
        glBindTexture(GL_TEXTURE_2D_ARRAY, tex_array_id);
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA, tex_width, tex_height, n_textures);
        std::cout << "Texture array ID: " << tex_array_id << " (max: " << GL_MAX_ARRAY_TEXTURE_LAYERS << ")\n";

        // Now we load textures and splat them into the texture
        for (const auto &tex : textures) {
            const int texture_id = std::get<0>(tex);
            const std::string texture_filename = std::get<1>(tex);
            const std::string normal_filename = std::get<2>(tex);
            const std::string specular_filename = std::get<3>(tex);
            const std::string displacement_filename = std::get<4>(tex);

            int tex_id = 0;
            int normal_id = 0;
            int specular_id = 0;
            int displacement_id = 0;
            bool has_normal = false;
            bool has_specular = false;
            bool has_displacement = false;
            if (!texture_filename.empty()) {
                tex_id = load_new_texture(texture_filename);
            }
            if (!normal_filename.empty()) {
                normal_id = load_new_texture(normal_filename);
                has_normal = true;
            }
            if (!specular_filename.empty()) {
                specular_id = load_new_texture(specular_filename);
                has_specular = true;
            }
            if (!displacement_filename.empty()) {
                displacement_id = load_new_texture(displacement_filename);
                has_displacement = true;
            }
            atlas[texture_id] = texture_t{tex_id, normal_id, has_normal, specular_id, has_specular, displacement_id, has_displacement};
            //std::cout << "Loaded texture " << texture_id << " as " << tex_id << " / " << normal_id << "\n";

            // Finally set some texture parameters
        }
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

    texture_t * get_texture_by_id(const int &id) {
        //std::cout << "Requesting texture atlas #" << id << "\n";
        auto finder = atlas.find(id);
        if (finder == atlas.end()) {
            //std::cout << "Returning default texture (request " << id << ")\n";
            return &atlas[1];
        }
        //std::cout << "Providing texture " << finder->second.texture_id << "\n";
        return &finder->second;
    }

}