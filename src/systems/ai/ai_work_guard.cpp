#include "ai_work_guard.hpp"
#include "../../components/ai_tag_work_guarding.hpp"
#include "../damage/weapons_helpers.hpp"
#include "../../main/game_globals.hpp"
#include "../../components/ai_tag_my_turn.hpp"
#include "path_finding.hpp"
#include "../../messages/renderables_changed_message.hpp"
#include "../../messages/entity_moved_message.hpp"
#include "ai_work_template.hpp"

void ai_work_guard::configure() {}

void ai_work_guard::update(const double duration_ms) {
    ai_work_template<ai_tag_work_guarding> work;

    work.do_ai([this, &work] (entity_t &e, ai_tag_work_guarding &g, ai_tag_my_turn_t &t, position_t &pos) {
        if (g.step == ai_tag_work_guarding::GOTO_POST) {
            if (!g.has_post) {
                //std::cout << "Selecting a post\n";
                // Select post and claim it
                float distance = std::numeric_limits<float>().max();
                std::size_t i = 0;
                std::size_t idx = std::numeric_limits<std::size_t>().max();
                for (auto &gp : designations->guard_points) {
                    if (!gp.first) {
                        float d = distance3d(pos.x, pos.y, pos.z, gp.second.x, gp.second.y, gp.second.z);
                        if (d < distance) {
                            distance = d;
                            idx = i;
                        } else {
                            std::cout << "Guardpost already claimed\n";
                        }
                    }
                    ++i;
                }

                std::cout << "Picked: " << idx << "\n";

                if (idx == std::numeric_limits<std::size_t>().max()) {
                    delete_component<ai_tag_work_guarding>(e.id);
                } else {
                    g.guard_post = designations->guard_points[idx].second;
                    designations->guard_points[idx].first = true;
                    g.has_post = true;
                }
                return;
            }

            if (!g.current_path) {
                // Determine a path - bail out if none
                g.current_path = find_path(pos, g.guard_post);
                if (!g.current_path->success) {
                    delete_component<ai_tag_work_guarding>(e.id);
                    for (auto &gp : designations->guard_points) {
                        if (gp.second == g.guard_post) gp.first = false;
                    }
                }
                //std::cout << "Found a path\n";
                return;
            } else {
                work.follow_path(g, pos, e, [&g, &e] () {
                    // Cancel
                    g.current_path.reset();
                    delete_component<ai_tag_work_guarding>(e.id);
                    for (auto &gp : designations->guard_points) {
                        if (gp.second == g.guard_post) gp.first = false;
                    }
                    return;
                }, [&g] () {
                    // Arrived
                    g.current_path.reset();
                    g.step = ai_tag_work_guarding::guard_steps::GUARD;
                    return;
                });
            }
        } else if (g.step == ai_tag_work_guarding::guard_steps::GUARD) {
            // Do we still have ammo? If not, stop guarding.
            if (shooting_range(e, pos) == 0) {
                delete_component<ai_tag_work_guarding>(e.id);
                for (auto &gp : designations->guard_points) {
                    if (gp.second == g.guard_post) gp.first = false;
                }
            }
        }
    });
}
