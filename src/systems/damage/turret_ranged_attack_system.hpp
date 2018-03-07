#pragma once

namespace systems {
	namespace turret_attacks {

		struct turret_ranged_attack_message {
			turret_ranged_attack_message() = default;
			turret_ranged_attack_message(const int attack, const int defend) : attacker(attack), victim(defend) {}

			int attacker;
			int victim;
		};

		void run(const double &duration_ms);
		void request_attack(turret_ranged_attack_message msg);
	}
}