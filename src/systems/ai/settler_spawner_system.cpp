#include "settler_spawner_system.hpp"
#include "../../planet/builder/settler_builder.hpp"
#include "../../messages/messages.hpp"
#include "../../main/game_planet.hpp"
#include "../../main/game_rng.hpp"
#include "../../main/game_logger.hpp"
#include "../../main/game_region.hpp"

void settler_spawner_system::configure() {
    system_name = "Settler Spawner";
    subscribe_mbox<day_elapsed_message>();
}

void settler_spawner_system::update(const double duration_ms) {
    std::queue<day_elapsed_message> * day = mbox<day_elapsed_message>();
    while (!day->empty()) {
        day->pop();

        // New arrivals
        if (planet.remaining_settlers > 0) {
            ++planet.migrant_counter;
            if (planet.migrant_counter > 14 && !planet.strict_beamdown) { // Every 2 weeks
                const int crash_x = REGION_WIDTH / 2;
                const int crash_y = REGION_HEIGHT / 2;
                const int crash_z = get_ground_z(*current_region, crash_x, crash_y);

                const std::vector<position_t> settler_arrival_points{
                        {crash_x - 2, crash_y - 2, crash_z+1},
                        {crash_x - 1, crash_y - 2, crash_z+1},
                        {crash_x, crash_y - 2, crash_z+1},
                        {crash_x + 1, crash_y - 2, crash_z+1},
                        {crash_x - 3, crash_y, crash_z+1},
                        {crash_x - 2, crash_y, crash_z+1},
                        {crash_x - 1, crash_y, crash_z+1},
                        {crash_x, crash_y, crash_z+1},
                        {crash_x + 1, crash_y, crash_z+1}
                };


                planet.migrant_counter = 0;
                const int new_settler_count = std::min(planet.remaining_settlers, rng.roll_dice(1,6));
                planet.remaining_settlers -= new_settler_count;
                std::stringstream ss;
                ss << "Ark auto-repair has managed to make the teleporter work! " << new_settler_count << " settlers have beamed down.";
                emit_deferred(log_message{LOG{}.text(ss.str())->chars});

                for (auto i=0; i<new_settler_count; ++i) {
                    const position_t spawn_point = settler_arrival_points[i % settler_arrival_points.size()];
                    create_settler(planet, spawn_point.x, spawn_point.y, spawn_point.z, rng, rng.roll_dice(1,3)-1);
                }
            }
        }
    }
}