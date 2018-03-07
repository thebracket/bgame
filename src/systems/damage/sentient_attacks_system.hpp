#pragma once

namespace systems {
	namespace sentient_attacks {

		struct sentient_attack_message {
			sentient_attack_message() = default;
			sentient_attack_message(const int attack, const int defend) : attacker(attack), victim(defend) {}

			int attacker;
			int victim;
		};

		struct sentient_ranged_attack_message {
			sentient_ranged_attack_message() = default;
			sentient_ranged_attack_message(const int attack, const int defend) : attacker(attack), victim(defend) {}

			int attacker;
			int victim;
		};

		void run(const double &duration_ms);
		void request_attack(sentient_attack_message msg);
		void request_ranged_attack(sentient_ranged_attack_message msg);
	}
}