#pragma once

#include <memory>

struct ai_tag_work_harvest {

    enum harvest_steps { FIND_HOE, FETCH_HOE, FIND_HARVEST, GOTO_HARVEST, DO_HARVEST };

    harvest_steps step = FIND_HOE;
	std::size_t tool_id = 0;
	std::shared_ptr<navigation_path_t> current_path; // Not serialized

};

