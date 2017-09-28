#pragma once

#include <vector>
#include "../../bengine/rexspeeder.hpp"

namespace systems {

	namespace logging {
		struct log_message {
			log_message() {}
			log_message(std::vector<xp::vchar> &line) : chars(line) {}
			std::vector<xp::vchar> chars;
		};

		void log(log_message &message);
		void run(const double &duration_ms);
		void age_log();
	}
}