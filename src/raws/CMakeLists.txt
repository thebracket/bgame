add_library(raws STATIC
		biomes.cpp
		buildings_raw.cpp
		clothing.cpp
		creatures.cpp
		health_factory.cpp
		items.cpp
		life_events.cpp
		lua_bridge.cpp
		materials.cpp
		plants.cpp
		profession.cpp
		raws.cpp
		reactions.cpp
		species_raw.cpp
		string_table.cpp
)
target_link_libraries(raws ${LUA_LIBRARIES} globalassets  )
cotire(raws)

