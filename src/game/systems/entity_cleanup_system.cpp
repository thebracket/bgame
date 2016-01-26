#include "entity_cleanup_system.hpp"
#include "../game.h"
#include <unordered_set>

void entity_cleanup_system::tick(const double& duration_ms) {
	std::unordered_set<int> deleted_entities;

	// Scan for deleted components
	const vector<position_component3d> * positions = ECS->find_components_by_type<position_component3d>();
	for (const position_component3d &pos : *positions) {
		if (pos.deleted) {
			deleted_entities.insert(pos.entity_id);
		}
	}

	// Scan the parent entities for anything pointing to the deleted entities
	const vector<parent_entity_component> * parents = ECS->find_components_by_type<parent_entity_component>();
	for (const parent_entity_component &parent : *parents) {
		auto finder = deleted_entities.find(parent.parent_id);
		if (finder != deleted_entities.end()) {
			ECS->delete_entity(parent.parent_id);
		}
	}
}
