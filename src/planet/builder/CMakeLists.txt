add_subdirectory(regions)
add_library(builder STATIC
        settler_builder.cpp
        map_builder.cpp
        biome_builder.cpp
        region_builder.cpp
        history_builder.cpp
        sentient_builder.cpp
        river_builder.cpp
        )
target_link_libraries(builder rltk raws FastNoise regions)
cotire(builder)
