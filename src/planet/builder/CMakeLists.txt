add_subdirectory(regions)
add_library(builder STATIC
        settler_builder.cpp
        map_builder.cpp
        biome_builder.cpp
        region_builder.cpp
        history_builder.cpp
        sentient_builder.cpp
        river_builder.cpp
        regions/starting_point.cpp
        regions/heightmap.cpp
        regions/water_features.cpp
        regions/strata.cpp
        regions/decorators.cpp
        regions/trees.cpp
        regions/buildings.cpp
        )
target_link_libraries(builder rltk raws FastNoise)
cotire(builder)
