#pragma once

#include "movement_system.hpp"

namespace systems {
	namespace triggers {
		void run(const double &duration_ms);
		void entry_trigger_firing(const systems::movement::entity_moved_message &msg);
	}
}