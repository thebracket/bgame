#include "ai_work_time.hpp"
#include "../../components/ai_tag_work_shift.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/position.hpp"
#include "distance_map_system.hpp"
#include "../../main/game_globals.hpp"
#include "../../components/ai_mode_idle.hpp"
#include "../../components/ai_tag_work_lumberjack.hpp"
#include "../../components/ai_tag_work_mining.hpp"
#include "../../components/ai_tag_work_guarding.hpp"
#include "../damage/weapons_helpers.hpp"
#include "mining_system.hpp"
#include <map>

namespace jobs_board {
    enum job_type_t { LUMBERJACK, MINING, GUARD };

    void evaluate_lumberjacking(std::map<int, job_type_t> &board, entity_t &e, position_t &pos) {
        if (designations->chopping.empty()) return; // Nothing to cut down

        auto axe_distance = axe_map.get(mapidx(pos));
        if (axe_distance > MAX_DIJSTRA_DISTANCE-1) return; // No axe available

        // Evaluate the closest tree to chop
        std::size_t i = 0;
        float distance = std::numeric_limits<float>().max();
        std::size_t selected = 0;
        for (const auto &chop : designations->chopping) {
            const float d = distance3d(pos.x, pos.y, pos.z, chop.second.x, chop.second.y, chop.second.z);
            if (d < distance) {
                distance = d;
                selected = i;
            }
            ++i;
        }

        board.insert(std::make_pair(distance + axe_distance, LUMBERJACK));
    }

    void evaluate_mining(std::map<int, job_type_t> &board, entity_t &e, position_t &pos) {
        if (designations->mining.empty()) return; // No mining to do

        auto pick_distance = pick_map.get(mapidx(pos));
        if (pick_distance > MAX_DIJSTRA_DISTANCE-1) return; // No pick available

        const auto idx = mapidx(pos);
        const int distance = mining_map[idx] + pick_distance;

        board.insert(std::make_pair(distance, MINING));
    }

    void evaluate_guarding(std::map<int, job_type_t> &board, entity_t &e, position_t &pos) {
        if (designations->guard_points.empty()) return; // Nothing to guard
        if (shooting_range(e, pos)<1) return; // No gun
        for (const auto &g : designations->guard_points) {
            if (!g.first) {
                board.insert(std::make_pair(static_cast<int>(distance3d(pos.x, pos.y, pos.z, g.second.x, g.second.y, g.second.z)), GUARD));
            }
        }
    }

    std::map<int, job_type_t> job_evaluations(entity_t &e, position_t &pos) {
        std::map<int, job_type_t> board;

        evaluate_lumberjacking(board, e, pos);
        evaluate_mining(board, e, pos);
        evaluate_guarding(board, e, pos);

        return board;
    }
}

void ai_work_time::configure() {

}

void ai_work_time::update(const double duration_ms) {

    each<settler_ai_t, ai_tag_work_shift_t, position_t>([] (entity_t &e, settler_ai_t &ai, ai_tag_work_shift_t &work, position_t &pos) {
        // Do we already have a job? If so, then return to doing it!
        if (e.component<ai_tag_work_lumberjack>() != nullptr) return;
        if (e.component<ai_tag_work_miner>() != nullptr) return;
        if (e.component<ai_tag_work_guarding>() != nullptr) return;

        // Build a job candidates list, goal is to pick the easiest job to complete.
        auto available_jobs = jobs_board::job_evaluations(e, pos);

        if (available_jobs.empty()) {
            std::cout << "No available jobs\n";
            // There is nothing to do!
            e.assign(ai_mode_idle_t{});
            return;
        } else {
            delete_component<ai_tag_work_shift_t>(e.id);
        }

        auto job_type = available_jobs.begin()->second;
        switch (job_type) {
            case jobs_board::LUMBERJACK : { e.assign(ai_tag_work_lumberjack{}); } break;
            case jobs_board::MINING : { e.assign(ai_tag_work_miner{}); } break;
            case jobs_board::GUARD : { e.assign(ai_tag_work_guarding{}); } break;
        }
    });
}