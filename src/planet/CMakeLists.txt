add_library(planet
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
)
target_link_libraries(planet rltk raws FastNoise)
