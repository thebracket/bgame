#pragma once

#include "../../components/settler_ai.hpp"
#include "../../components/ai_tags/ai_tag_work_order.hpp"
#include "../../components/helpers/reaction_task_t.hpp"
#include <memory>

namespace bengine {
	struct entity_t;
}

namespace workflow {
	bool is_auto_reaction_task_available(const bengine::entity_t &worker, const settler_ai_t &ai) noexcept;
	std::unique_ptr<reaction_task_t> find_automatic_reaction_task(const bengine::entity_t &worker, const ai_tag_work_order &ai) noexcept;
	std::unique_ptr<reaction_task_t> find_queued_reaction_task(const bengine::entity_t &worker, const ai_tag_work_order &ai) noexcept;
	void free_workshop(const std::size_t &id) noexcept;

	extern std::unordered_map<std::size_t, std::vector<std::string>> automatic_reactions;
}
