#include "terrain_chunk_shader.hpp"

namespace gl {
    terrain_chunk_shader_t::terrain_chunk_shader_t() {
        load("world_defs/shaders/terrain_vertex.glsl",
             "world_defs/shaders/terrain_fragment.glsl");

        in_position_loc = get_attribute_location("in_position");
        world_position_loc = get_attribute_location("world_position");
        color_loc = get_attribute_location("color");
        texture_position_loc = get_attribute_location("texture_position");
        projection_matrix_loc = get_uniform_location("projection_matrix");
        view_matrix_loc = get_uniform_location("view_matrix");
        my_color_texture_loc = get_uniform_location("my_color_texture");
        flags_loc = get_attribute_location("flags");
        camera_position_loc = get_uniform_location("camera_position");
        my_normal_texture_loc = get_uniform_location("my_normal_texture");
        my_specular_texture_loc = get_uniform_location("my_specular_texture");
        rotation_loc = get_attribute_location("rotation");
    }
}
