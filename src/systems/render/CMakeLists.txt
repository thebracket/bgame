add_library(render STATIC
        map_render_system.cpp
        camera_system.cpp
        lighting_system.cpp
        renderables_system.cpp
        )
target_link_libraries(render rltk bfmain)
cotire(render)
