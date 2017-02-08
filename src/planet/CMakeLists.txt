add_subdirectory(builder)
add_library(planet STATIC
    planet.cpp
	civilizations.cpp
	history.cpp
	region.cpp
	planet_builder.cpp
)
target_link_libraries(planet builder rltk raws FastNoise)
cotire(planet)
