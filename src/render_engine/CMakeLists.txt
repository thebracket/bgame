add_library(render_engine STATIC
        render_engine.cpp
        )
cotire(render_engine)
add_subdirectory(chunks)
target_link_libraries(render_engine chunks)