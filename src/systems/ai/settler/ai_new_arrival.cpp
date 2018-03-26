#include "stdafx.h"
#include "ai_new_arrival.hpp"
#include "../../../global_assets/rng.hpp"
#include "../../../raws/string_table.hpp"
#include "../../gui/log_system.hpp"

namespace systems {
	namespace ai_new_arrival {

		using namespace bengine;

		void run(const double &duration_ms) {
			each<ai_tag_my_turn_t, ai_settler_new_arrival_t>([](entity_t &e, ai_tag_my_turn_t &t, ai_settler_new_arrival_t &arrival) {
				delete_component<ai_tag_my_turn_t>(e.id); // No more turns

				auto settler = e.component<settler_ai_t>();
				if (settler) {
					settler->job_status = "New Arrival";
				}

				if (arrival.turns_since_arrival == 0 && rng.roll_dice(1, 6) == 1) {
					const std::string quip = string_tables::string_table(string_tables::NEW_ARRIVAL_QUIPS)->random_entry(rng);
					logging::log_message msg{ LOG{}.settler_name(e.id)->text("says ")->col(color_t{1.0f, 1.0f, 0.0f})->text(quip)->chars };
					logging::log(msg);
				}

				++arrival.turns_since_arrival;
				if (arrival.turns_since_arrival > 10) {
					delete_component<ai_settler_new_arrival_t>(e.id);
					e.assign(ai_mode_idle_t{});
				}
			});
		}
	}
}
