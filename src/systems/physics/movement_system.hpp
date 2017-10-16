#pragma once

#include "../../components/position.hpp"
#include "../../bengine/ecs.hpp"

namespace systems {
	namespace movement {
		struct entity_moved_message {
			entity_moved_message() {}
			entity_moved_message(std::size_t id, const position_t &orig, const position_t &dest) : entity_id(id), origin(orig), destination(dest) {}
			std::size_t entity_id;
			position_t origin;
			position_t destination;
		};

		void move_to(bengine::entity_t &e, position_t &pos, position_t &dest);
		void move_to(const std::size_t id, const position_t &dest);
		void request_flee(std::size_t id, std::size_t flee_from);
		void request_charge(std::size_t id, std::size_t charge_to);
		void request_random_move(std::size_t id);

		void run(const double &duration_ms);
	}
}