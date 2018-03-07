#pragma once

#include "../../components/position.hpp"
#include "../../bengine/ecs.hpp"

namespace systems {
	namespace movement {
		struct entity_moved_message {
			entity_moved_message() {}
			entity_moved_message(const int id, const position_t &orig, const position_t &dest) : entity_id(id), origin(orig), destination(dest) {}
			int entity_id;
			position_t origin;
			position_t destination;
		};

		void move_to(bengine::entity_t &e, const position_t &pos, const position_t &dest);
		void request_flee(int id, int flee_from);
		void request_charge(int id, int charge_to);
		void request_random_move(int id);

		void run(const double &duration_ms);
	}
}