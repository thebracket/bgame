add_library(gl STATIC gl_utils.cpp
        worldgen_render.cpp
        map_render.cpp
        chunk.cpp
        main_fbo.cpp
        sun_moon.cpp
        populate_gbuffer.cpp
        composition.cpp
        )
target_link_libraries(gl rltk ${SFML_LIBRARIES} render input raws bfmain region)
#cotire(gl)
