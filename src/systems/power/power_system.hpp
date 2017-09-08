#pragma once

#include <string>

namespace systems {
	namespace power {
		struct consumed_power_t {
			std::string source = "";
			int amount = 0;
		};

		void run(const double &duration_ms);
		void consume_power(consumed_power_t drain);
	}
}