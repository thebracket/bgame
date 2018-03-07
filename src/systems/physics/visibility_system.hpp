#pragma once

#include <unordered_set>

namespace systems {
	namespace visibility {

		extern std::unordered_set<std::size_t> blocked_visibility;

		void run(const double &duration_ms);
		void opacity_is_dirty();
		void on_entity_moved(int &entity_id);
	}
}