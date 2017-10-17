#pragma once

namespace systems {
	namespace turret_attacks {

		struct turret_ranged_attack_message {
			turret_ranged_attack_message() {}
			turret_ranged_attack_message(const size_t attack, const size_t defend) : attacker(attack), victim(defend) {}

			size_t attacker;
			size_t victim;
		};

		void run(const double &duration_ms);
		void request_attack(turret_ranged_attack_message msg);
	}
}