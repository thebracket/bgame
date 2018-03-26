#pragma once

#include "../../components/settler_ai.hpp"
#include "../../components/ai_tags/ai_tag_work_order.hpp"
#include "../../components/helpers/reaction_task_t.hpp"
#include <memory>

namespace bengine {
	class entity_t;
}

namespace workflow {
	bool is_auto_reaction_task_available(const int worker_id, const settler_ai_t &ai) noexcept;
	std::unique_ptr<reaction_task_t> find_automatic_reaction_task(const int worker_id, const ai_tag_work_order &ai) noexcept;
	std::unique_ptr<reaction_task_t> find_queued_reaction_task(const int worker_id, const ai_tag_work_order &ai) noexcept;

	extern std::unordered_map<std::size_t, std::vector<std::string>> automatic_reactions;
}
