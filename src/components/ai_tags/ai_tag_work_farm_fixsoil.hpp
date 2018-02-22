#pragma once

#include "../../systems/helpers/pathfinding.hpp"
#include <memory>

struct ai_tag_work_farm_fixsoil {

    enum fix_steps { FIND_HOE, FETCH_HOE, FIND_SOIL, FETCH_SOIL, FIND_TARGET, GOTO_TARGET, FIX_TARGET };

	fix_steps step = FIND_HOE;
	std::size_t tool_id = 0;
	std::size_t soil_id = 0;
	std::shared_ptr<navigation_path_t> current_path; // Not serialized
};

