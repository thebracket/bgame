#pragma once

namespace systems {
	namespace sentient_attacks {

		struct sentient_attack_message {
			sentient_attack_message() {}
			sentient_attack_message(const size_t attack, const size_t defend) : attacker(attack), victim(defend) {}

			size_t attacker;
			size_t victim;
		};

		struct sentient_ranged_attack_message {
			sentient_ranged_attack_message() {}
			sentient_ranged_attack_message(const size_t attack, const size_t defend) : attacker(attack), victim(defend) {}

			size_t attacker;
			size_t victim;
		};

		void run(const double &duration_ms);
		void request_attack(sentient_attack_message msg);
		void request_ranged_attack(sentient_ranged_attack_message msg);
	}
}