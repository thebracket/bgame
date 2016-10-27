add_library(planet STATIC
    planet.cpp
	civilizations.cpp
	history.cpp
	region.cpp
	planet_builder.cpp
	builder/settler_builder.cpp
	builder/map_builder.cpp
	builder/biome_builder.cpp
	builder/region_builder.cpp
	builder/history_builder.cpp
	builder/sentient_builder.cpp
	builder/river_builder.cpp
	builder/regions/starting_point.cpp
	builder/regions/heightmap.cpp
	builder/regions/water_features.cpp
	builder/regions/strata.cpp
	builder/regions/decorators.cpp
)
target_link_libraries(planet rltk raws FastNoise)
