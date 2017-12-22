add_library(render_engine
        camera.cpp
        design_render.cpp
        pointlights.cpp
        render_engine.cpp
        renderbuffers.cpp
        ssao.cpp
        )
#cotire(render_engine)
add_subdirectory(chunks)
add_subdirectory(fbo)
add_subdirectory(vox)
add_subdirectory(shaders)
target_link_libraries(render_engine chunks fbo vox shaders)
cotire(render_engine)