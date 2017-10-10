#pragma once

#include "../../components/settler_ai.hpp"
#include "../../components/ai_tags/ai_tag_work_order.hpp"
#include <memory>

namespace workflow {
	bool is_auto_reaction_task_available(const settler_ai_t &ai);
	std::unique_ptr<reaction_task_t> find_automatic_reaction_task(const ai_tag_work_order &ai);
	std::unique_ptr<reaction_task_t> find_queued_reaction_task(const ai_tag_work_order &ai);
	void free_workshop(const std::size_t &id);
	void clear_automatic_reactions();

	extern std::unordered_map<std::size_t, std::vector<std::string>> automatic_reactions;
	extern std::unordered_set<std::size_t> workshop_claimed;
}