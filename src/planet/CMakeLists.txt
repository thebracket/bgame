add_subdirectory(builder)
add_subdirectory(region)
add_library(planet STATIC
    planet.cpp
	planet_builder.cpp
)
target_link_libraries(planet builder rltk raws FastNoise)
cotire(planet)
