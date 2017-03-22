add_library(gl gl_utils.cpp
        worldgen_render.cpp
        map_render.cpp
        world_scene.cpp
        frustrum.cpp
        render_block.cpp
        )
cotire(gl)
