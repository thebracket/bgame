add_library(chunks STATIC
        chunk.cpp
        geometry_buffer.cpp
        terrain_bucket.cpp
)
target_link_libraries(chunks shaders)
