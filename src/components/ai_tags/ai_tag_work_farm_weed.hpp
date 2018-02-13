#pragma once

#include "../position.hpp"
#include "../../bengine/path_finding.hpp"
#include <memory>

struct ai_tag_work_farm_weed {

    enum weed_steps { FIND_HOE, FETCH_HOE, FIND_TARGET, FETCH_TARGET, WEED };

	weed_steps step = FIND_HOE;
	std::size_t tool_id = 0;
	std::shared_ptr<bengine::navigation_path<position_t>> current_path; // Not serialized
};

