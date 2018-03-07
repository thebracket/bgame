#pragma once

#include "../position.hpp"
#include <memory>
#include "../../systems/helpers/pathfinding.hpp"

struct ai_tag_work_farm_plant {

    enum plant_steps { FIND_HOE, FETCH_HOE, FIND_SEED, FETCH_SEED, FIND_TARGET, FETCH_TARGET, PLANT };

	plant_steps step = FIND_HOE;
	int tool_id = 0;
	int seed_id = 0;
	position_t farm_position;
	std::shared_ptr<navigation_path_t> current_path; // Not serialized
};

