#pragma once

#include "../../components/position.hpp"
#include "../../bengine/ecs.hpp"

namespace systems {
	namespace movement {
		void move_to(bengine::entity_t &e, position_t &pos, position_t &dest);

		void run(const double &duration_ms);
	}
}