#include "world_queries.hpp"
#include <rltk.hpp>
#include "../../components/building.hpp"
#include "../../components/corpse_harvestable.hpp"

using namespace rltk;

bool butcher_exist() {
	bool butcher_exists = false;

	each<building_t>([&butcher_exists] (entity_t &e, building_t &b) {
		if (b.tag == "butcher" && b.complete == true) butcher_exists = true;
	});

	return butcher_exists;
}
