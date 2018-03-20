#pragma once

struct ai_tag_work_pull_lever {

    enum lever_steps { FIND, PULL };

    lever_steps step = FIND;
	std::shared_ptr<navigation_path_t> current_path; // Not serialized
};

