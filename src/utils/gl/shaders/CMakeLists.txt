add_library(shaders STATIC
        base_shader.cpp
        shader_storage.cpp
        terrain_chunk_shader.cpp
        static_model_shader.cpp
        renderable_shader.cpp
        render_shader.cpp
        final_shader.cpp
)
cotire(shaders)
