#pragma once

namespace systems {
	namespace settler_melee_attack {

		struct settler_attack_message {
			settler_attack_message() = default;
			settler_attack_message(const int attack, const int defend) : attacker(attack), victim(defend) {}

			int attacker;
			int victim;
		};

		void run(const double &duration_ms);
		void request_melee(settler_attack_message msg);
	}
}