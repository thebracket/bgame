#pragma once

namespace systems {
	namespace creature_attacks {

		struct creature_attack_message {
			creature_attack_message() {}
			creature_attack_message(const size_t attack, const size_t defend) : attacker(attack), victim(defend) {}

			size_t attacker;
			size_t victim;
		};

		void run(const double &duration_ms);
		void request_attack(creature_attack_message msg);
	}
}