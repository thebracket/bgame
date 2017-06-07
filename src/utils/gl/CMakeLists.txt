add_library(gl STATIC gl_utils.cpp
        worldgen_render.cpp
        map_render.cpp
        world_scene.cpp
        render_block.cpp
        chunk.cpp
        main_fbo.cpp
        sun_fbo.cpp
        )
target_link_libraries(gl rltk ${SFML_LIBRARIES} render input raws bfmain region)
