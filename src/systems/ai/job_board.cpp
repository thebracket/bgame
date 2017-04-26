#include "job_board.hpp"
#include <vector>
#include <algorithm>

namespace jobs_board {

    namespace impl {
        std::vector<job_evaluator_t> evaluators;
    }

    void register_job_offer(const job_evaluator_t &&evaluator) {
        impl::evaluators.emplace_back(evaluator);
    }

    void evaluate(job_board_t &board, rltk::entity_t &entity, position_t &pos) {
        std::for_each(impl::evaluators.begin(), impl::evaluators.end(),
            [&board, &entity, &pos] (const job_evaluator_t &func) {
                func(board, entity, pos);
        });
    }
}