#pragma once

#include <vector>

namespace systems {
	namespace inventory_system {
		void run(const double &duration_ms);
		void drop_item(const std::size_t &ID, const int &X, const int &Y, const int &Z);
	}
}