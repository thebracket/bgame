#include "jobs_board.hpp"

namespace jobs_board {

	namespace impl {
		std::vector<std::unique_ptr<job_evaluator_base_t>> evaluators;
	}

	bool is_working(bengine::entity_t &e) {
		for (const auto &job : impl::evaluators) {
			if (job->has_tag(e)) return true;
		}
		return false;
	}

	void evaluate(job_board_t &board, bengine::entity_t &entity, position_t &pos) {
		std::for_each(impl::evaluators.begin(), impl::evaluators.end(),
			[&board, &entity, &pos](const auto &j) {
			j->exec(board, entity, pos);
		});
	}
}
