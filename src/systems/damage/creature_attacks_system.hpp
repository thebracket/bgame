#pragma once

namespace systems {
	namespace creature_attacks {

		struct creature_attack_message {
			creature_attack_message() {}
			creature_attack_message(const int attack, const int defend) : attacker(attack), victim(defend) {}

			int attacker;
			int victim;
		};

		void run(const double &duration_ms);
		void request_attack(creature_attack_message msg);
	}
}