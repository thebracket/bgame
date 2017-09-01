add_library(render_engine
        render_engine.cpp
        )
#cotire(render_engine)
add_subdirectory(chunks)
add_subdirectory(fbo)
add_subdirectory(world_textures)
add_subdirectory(vox)
add_subdirectory(shaders)
target_link_libraries(render_engine chunks fbo world_textures vox shaders)