#pragma once

#include "../../components/position.hpp"
#include "../../bengine/ecs.hpp"

namespace systems {
	namespace movement {
		void move_to(bengine::entity_t &e, position_t &pos, position_t &dest);
		void request_flee(std::size_t id, std::size_t flee_from);
		void request_charge(std::size_t id, std::size_t charge_to);

		void run(const double &duration_ms);
	}
}