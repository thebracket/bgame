#include "ai_work_hunt.hpp"
#include "../../../components/ai_tags/ai_tag_work_hunting.hpp"
#include "../../../components/building.hpp"
#include "job_board.hpp"
#include "../distance_map_system.hpp"
#include "../../../planet/region/region.hpp"
#include "ai_work_template.hpp"

namespace jobs_board {
    bool butcher_exist() {
        bool butcher_exists = false;

        each<building_t>([&butcher_exists] (entity_t &e, building_t &b) {
            if (b.tag == "butcher" && b.complete == true) butcher_exists = true;
        });

        return butcher_exists;
    }

    void evaluate_hunting(job_board_t &board, entity_t &e, position_t &pos, job_evaluator_base_t *jt) {
        if (!butcher_exist()) return;
        const auto distance = huntables_map.get(mapidx(pos));
        if (distance > MAX_DIJSTRA_DISTANCE-1) return;
        board.insert(std::make_pair( distance, jt ));
    }
}

void ai_work_hunt::configure() {
    using namespace jobs_board;
    register_job_offer<ai_tag_work_hunting>(evaluate_hunting);
}

void ai_work_hunt::update(const double duration_ms) {
    ai_work_template<ai_tag_work_hunting> work;

    work.do_ai([this, &work] (entity_t &e, ai_tag_work_hunting &h, ai_tag_my_turn_t &t, position_t &pos) {
        work.set_status(e, "Hunting");
        const auto distance = huntables_map.get(mapidx(pos));
        if (distance > MAX_DIJSTRA_DISTANCE-1) {
            work.cancel_work_tag(e);
            return;
        }

        work.folllow_path(huntables_map, pos, e, [&e, &work] () {
            // Cancel
            work.cancel_work_tag(e);
            return;
        }, [&e, &work] () {
            // At destination - stop hunting
            work.cancel_work_tag(e);
            return;
        });
    });
}
