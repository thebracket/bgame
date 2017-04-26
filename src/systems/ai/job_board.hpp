#pragma once

#include <functional>
#include "../../components/position.hpp"
#include <rltk.hpp>
#include <map>

namespace jobs_board {
    enum job_type_t { LUMBERJACK, MINING, GUARD };

    using job_board_t = std::map<int, job_type_t>;
    using job_evaluator_t = std::function<void(job_board_t &, rltk::entity_t &, position_t &)>;
    void register_job_offer(const job_evaluator_t &&evaluator);
    void evaluate(job_board_t &board, rltk::entity_t &entity, position_t &pos);
}