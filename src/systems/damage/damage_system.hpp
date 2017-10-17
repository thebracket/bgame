#pragma once

#include <string>

namespace systems {
	namespace damage_system {
		struct inflict_damage_message {
			inflict_damage_message() {}
			inflict_damage_message(std::size_t target, int amount, std::string type) : victim(target), damage_amount(amount), damage_type(type) {}

			std::size_t victim;
			int damage_amount;
			std::string damage_type;
		};

		void run(const double &duration_ms);
		void inflict_damage(inflict_damage_message msg);
	}
}