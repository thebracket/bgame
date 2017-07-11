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
#include <SFML/Graphics.hpp>

namespace textures {

    sf::RenderTexture atlas_texture;
    unsigned int next_texture_id = 0;

    boost::container::flat_map<int, texture_t> atlas;

    void bind_atlas() {
        sf::Texture::bind(&atlas_texture.getTexture());
    }

    int load_new_texture(const std::string filename) {
        sf::Texture tex;
        tex.loadFromFile(filename);

        sf::Vector2f atlas_position((next_texture_id % SHEET_CHARS) * TEX_IN_ATLAS_WIDTH, (next_texture_id / SHEET_CHARS) * TEX_IN_ATLAS_HEIGHT);
        std::cout << "Selected atlas position: " << atlas_position.x << " / " << atlas_position.y << "; " << filename << "\n";

        auto size = tex.getSize();
        float scale_x = 1.0f;
        float scale_y = 1.0f;
        bool rescale = false;
        if (size.x != (float)TEX_IN_ATLAS_WIDTH) {
            scale_x = size.x / (float)TEX_IN_ATLAS_WIDTH;
            std::cout << "Warning: X scaling " << filename << "\n";
            rescale = true;
        }
        if (size.y != (float)TEX_IN_ATLAS_HEIGHT) {
            scale_y = size.y / (float)TEX_IN_ATLAS_HEIGHT;
            std::cout << "Warning: Y scaling " << filename << "\n";
            rescale = true;
        }

        sf::Sprite sprite(tex);
        sprite.setPosition(atlas_position);
        if (rescale) sprite.setScale(scale_x, scale_y);
        atlas_texture.draw(sprite);

        int id = next_texture_id;
        ++next_texture_id;
        return id;
    }

    void load_textures(std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> textures)
    {
        // First we create the backing texture
        atlas_texture.create(ATLAS_WIDTH, ATLAS_HEIGHT);
        atlas_texture.clear();

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
            bind_atlas();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);

        }
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