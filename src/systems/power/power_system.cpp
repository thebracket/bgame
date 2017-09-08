#include "power_system.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../global_assets/game_calendar.hpp"
#include "../../components/construct_power.hpp"
#include "../../bengine/ecs.hpp"
#include "../../render_engine/world_textures/world_textures.hpp"
#include <thread>
#include <mutex>
#include <queue>

namespace systems {
	namespace power {
		std::mutex power_mutex;
		std::queue<consumed_power_t> drains;

		void consume_power(consumed_power_t drain) {
			std::lock_guard<std::mutex> lock(power_mutex);
			drains.push(drain);
		}

		void run(const double &duration_ms) {
			using namespace bengine;

			const int starting_power = designations->current_power;

			designations->total_capacity = 0;
			int total_always_on = 0;
			int total_solar = 0;

			each<construct_power_t>([&total_always_on, &total_solar](entity_t &e, construct_power_t &p) {
				designations->total_capacity += p.storage_capacity;
				total_always_on += p.generation_always;
				total_solar += p.generation_solar;
			});

			designations->current_power += total_always_on;
			if (calendar->hour > 6 && calendar->hour < 19) designations->current_power += total_solar;
			if (designations->current_power > designations->total_capacity) designations->current_power = designations->total_capacity;

			int consumption = 0;
			std::lock_guard<std::mutex> lock(power_mutex);
			while (!drains.empty()) {
				auto drain = drains.front();
				drains.pop();
				consumption += drain.amount;
			}

			designations->current_power -= consumption;

			if (starting_power != designations->current_power) render::light_changed = true;
		}
	}
}