add_library(chunks STATIC
        chunk.cpp
        geometry_buffer.cpp
        terrain_bucket.cpp
        base_geometry.cpp
)
target_link_libraries(chunks shaders)
