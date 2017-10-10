#pragma once

#include <vector>
#include <bitset>
#include <unordered_map>
#include <unordered_set>

namespace systems {
	namespace workflow_system {
		void run(const double &duration_ms);
		void update_workflow();
	}
}