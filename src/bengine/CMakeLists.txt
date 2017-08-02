add_library(bengine STATIC
        main_window.cpp
        stb_image.cpp
        textures.cpp
        shaders.cpp
        simple_sprite.cpp
        imgui.cpp
        imgui_impl_glfw_gl3.cpp
        imgui_draw.cpp
        threadpool.cpp
        filesystem.cpp
        color_t.cpp
        pcg_basic.c
        random_number_generator.cpp
        string_utils.cpp
        telemetry.cpp
        ecs.cpp
        FastNoise.cpp
        geometry.cpp
        rexspeeder.cpp
        octree.cpp
        )
target_link_libraries(bengine ${ZLIB_LIBS})
cotire(bengine)