#pragma once

struct ai_tag_leisure_drink_t {

	enum drink_steps { FIND_DRINK, GOTO_DRINK, CONSUME_DRINK };

	drink_steps step = FIND_DRINK;
	std::size_t tool_id = 0;
	std::shared_ptr<navigation_path_t> current_path; // Not serialized
};

