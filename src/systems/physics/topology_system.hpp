#pragma once

#include <string>

namespace systems {
	namespace topology {
		void run(const double &duration_ms);
		void perform_mining(const int idx, const int op, const int x, const int y, const int z);
		void perform_construction(const size_t &entity, const std::string &t, const std::size_t &mat);
	}
}