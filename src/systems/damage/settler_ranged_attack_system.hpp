#pragma once

namespace systems {
	namespace settler_ranged_attack {

		struct settler_ranged_attack_message {
			settler_ranged_attack_message() = default;
			settler_ranged_attack_message(const int attack, const int defend) : attacker(attack), victim(defend) {}

			int attacker;
			int victim;
		};

		void run(const double &duration_ms);
		void request_settler_ranged_attack(settler_ranged_attack_message msg);
	}
}