#pragma once

namespace systems {
	namespace settler_melee_attack {

		struct settler_attack_message {
			settler_attack_message() {}
			settler_attack_message(const size_t attack, const size_t defend) : attacker(attack), victim(defend) {}

			size_t attacker;
			size_t victim;
		};

		void run(const double &duration_ms);
		void request_melee(settler_attack_message msg);
	}
}