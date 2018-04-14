#pragma once

struct ai_tag_leisure_eat_t {

	enum eat_steps { FIND_FOOD, GOTO_FOOD, EAT_FOOD };

	eat_steps step = FIND_FOOD;
	std::size_t tool_id = 0;
	std::shared_ptr<navigation_path_t> current_path; // Not serialized
};

