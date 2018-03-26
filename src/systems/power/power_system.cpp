#include "stdafx.h"
#include "power_system.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../global_assets/game_calendar.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../../bengine/color_t.hpp"

namespace systems {
	namespace power {
		struct consumed_power_t {
			std::string source = "";
			int amount = 0;
		};


		thread_safe_message_queue<consumed_power_t> drains;

		void consume_power(const std::string &source, const int amount) {
			drains.enqueue(consumed_power_t{ source, amount });
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
			drains.process_all([&consumption](consumed_power_t drain) {
				consumption += drain.amount;
			});

			designations->current_power -= consumption;

			if (designations->current_power != starting_power) {
				float power_percent = (float)designations->current_power / (float)designations->total_capacity;
				bengine::color_t alert_color;
				if (power_percent < 0.3) {
					alert_color = color_t(1.0f, 0.0f, 0.0f);
				}
				else if (power_percent < 0.5) {
					alert_color = color_t(1.0f, 0.5f, 0.0f);
				}
				else if (power_percent < 0.75) {
					alert_color = color_t(1.0f, 1.0f, 0.0f);
				}
				else {
					alert_color = { 1.0f, 1.0f, 1.0f };
				}
				bengine::each<lightsource_t>([&alert_color](bengine::entity_t &e, lightsource_t &l) {
					if (l.alert_status) {
						l.color = alert_color;
					}
				});
			}
		}
	}
}
