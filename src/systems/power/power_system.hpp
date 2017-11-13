#pragma once

#include "../../stdafx.h"

namespace systems {
	namespace power {
		void run(const double &duration_ms);
		void consume_power(const std::string &source, const int amount);
	}
}