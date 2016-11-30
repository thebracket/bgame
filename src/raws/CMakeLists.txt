add_library(raws STATIC
	raws.cpp
	lua_bridge.cpp
	health_factory.cpp
	string_table.cpp
    materials.cpp
    native_population.cpp
    creatures.cpp
    species.cpp
    biomes.cpp
    plants.cpp
    life_events.cpp
    clothing.cpp
    profession.cpp
    items.cpp
)
target_link_libraries(raws liblua)
target_link_libraries(raws rltk)
target_link_libraries(raws utils)
target_link_libraries(raws components)
