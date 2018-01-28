#pragma once

namespace systems {
	namespace design_mining {
		void run(const double &duration_ms);
		extern std::vector<std::pair<int, uint8_t>> mining_cursor_list;
	}
}