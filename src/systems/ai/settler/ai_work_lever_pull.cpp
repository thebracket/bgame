#include "ai_work_lever_pull.hpp"
#include "ai_work_template.hpp"
#include "../../../components/ai_tags/ai_tag_work_pull_lever.hpp"
#include "../../../main/game_designations.hpp"
#include "../../../components/lever.hpp"
#include "../distance_map_system.hpp"
#include "job_board.hpp"
#include "../../../messages/map_dirty_message.hpp"

namespace jobs_board {
    void evaluate_levers(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
        if (designations->levers_to_pull.empty()) return; // Nothing to cut down

        auto lever_distance = levers_map.get(mapidx(pos));
        if (lever_distance > MAX_DIJSTRA_DISTANCE-1) return; // Nothing to harvest

        board.insert(std::make_pair(lever_distance, jt));
    }
}

void ai_work_lever_pull::configure() {
    // Register with the jobs board
    jobs_board::register_job_offer<ai_tag_work_pull_lever>(jobs_board::evaluate_levers);
}

void ai_work_lever_pull::update(const double duration_ms) {
    ai_work_template<ai_tag_work_pull_lever> work;
    work.do_ai([this, &work] (entity_t &e, ai_tag_work_pull_lever &l, ai_tag_my_turn_t &t, position_t &pos) {
        if (l.step == ai_tag_work_pull_lever::lever_steps::FIND) {
            // Path towards the harvest
            work.folllow_path(levers_map, pos, e, [&e]() {
                // On cancel
                delete_component<ai_tag_work_pull_lever>(e.id);
                return;
            }, [&e, this, &pos, &l, &work] {
                // On success
                l.step = ai_tag_work_pull_lever::lever_steps::PULL;
            });
            return;
        } else if (l.step == ai_tag_work_pull_lever::lever_steps::PULL) {
            std::size_t lever_id = 0;
            each<lever_t, position_t>([&lever_id, &pos] (entity_t &E, lever_t &L, position_t &p) {
                if (p == pos) lever_id = E.id;
            });
            if (lever_id == 0) throw std::runtime_error("Unidentified lever!");

            // Remove from designations
            designations->levers_to_pull.erase(std::remove_if(
                    designations->levers_to_pull.begin(),
                    designations->levers_to_pull.end(),
                    [&lever_id] (auto n) { return n == lever_id; }
            ), designations->levers_to_pull.end());

            // Update the lever map
            emit_deferred(leverpull_changed_message{});

            // Actually pull the lever
            emit_deferred(lever_pulled_message{lever_id});

            // Idle
            delete_component<ai_tag_work_pull_lever>(e.id);
            return;
        }
    });
}