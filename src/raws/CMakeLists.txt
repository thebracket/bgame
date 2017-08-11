add_library(raws STATIC
	raws.cpp
	lua_bridge.cpp
	health_factory.cpp
	string_table.cpp
    materials.cpp
    creatures.cpp
    species.cpp
    biomes.cpp
    plants.cpp
    life_events.cpp
    clothing.cpp
    profession.cpp
    items.cpp
    buildings.cpp
    reactions.cpp
)
target_link_libraries(raws globalassets ${LUA_LIBRARIES} )

