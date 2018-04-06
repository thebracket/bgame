#pragma once

#include <string>

namespace systems {
	namespace topology {
		struct perform_mining_message {
			perform_mining_message() = default;
			perform_mining_message(const int idx, const uint8_t op, int X, int Y, int Z) : target_idx(idx),
				operation(op), x(X), y(Y), z(Z) {}
			int target_idx = 0;
			uint8_t operation = 0;
			int x = 0, y = 0, z = 0;
		};

		void run(const double &duration_ms);
		void perform_mining(const int idx, const int op, const int x, const int y, const int z);
		void perform_construction(const size_t &entity, const std::string &t, const std::size_t &mat);
		void spawn_mining_result(const perform_mining_message &e);
	}
}