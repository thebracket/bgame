add_library(render_engine
        render_engine.cpp
        )
cotire(render_engine)
add_subdirectory(chunks)
add_subdirectory(fbo)
target_link_libraries(render_engine chunks fbo)