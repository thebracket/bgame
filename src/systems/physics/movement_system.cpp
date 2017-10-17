#include "movement_system.hpp"
#include "../../planet/region/region.hpp"
#include "../../global_assets/rng.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../../components/slidemove.hpp"
#include "../../components/initiative.hpp"
#include "../../components/riding_t.hpp"
#include "../../global_assets/spatial_db.hpp"
#include "trigger_system.hpp"
#include "visibility_system.hpp"

namespace systems {
	namespace movement {

		struct entity_wants_to_move_message {
			entity_wants_to_move_message() {}
			entity_wants_to_move_message(std::size_t id, const position_t dest) : entity_id(id), destination(dest) { }
			std::size_t entity_id;
			position_t destination;
		};

		struct entity_wants_to_move_randomly_message {
			entity_wants_to_move_randomly_message() {}
			entity_wants_to_move_randomly_message(std::size_t id) : entity_id(id) {}
			std::size_t entity_id;
		};

		struct entity_wants_to_flee_message {
			entity_wants_to_flee_message() {}
			entity_wants_to_flee_message(std::size_t id, std::size_t flee_from) : entity_id(id), flee_from_id(flee_from) {}
			std::size_t entity_id;
			std::size_t flee_from_id;
		};

		struct entity_wants_to_charge_message {
			entity_wants_to_charge_message() {}
			entity_wants_to_charge_message(std::size_t id, std::size_t charge_to) : entity_id(id), charge_to_id(charge_to) {}
			std::size_t entity_id;
			std::size_t charge_to_id;
		};		

		thread_safe_message_queue<entity_wants_to_move_message> move_requests;
		thread_safe_message_queue<entity_wants_to_move_randomly_message> wander_requests;
		thread_safe_message_queue<entity_wants_to_charge_message> charge_requests;
		thread_safe_message_queue<entity_wants_to_flee_message> flee_requests;
		thread_safe_message_queue<entity_moved_message> move_completions;

		void move_to(bengine::entity_t &e, position_t &pos, position_t &dest) {
			move_requests.enqueue(entity_wants_to_move_message{ e.id, dest });
		}

		void move_to(const std::size_t id, const position_t &dest) {
			move_requests.enqueue(entity_wants_to_move_message{ id, dest });
		}

		void request_move(entity_wants_to_move_message &msg) {
			move_requests.enqueue(std::move(msg));
		}

		void request_flee(std::size_t id, std::size_t flee_from) {
			flee_requests.enqueue(entity_wants_to_flee_message{id, flee_from});
		}

		void request_charge(std::size_t id, std::size_t charge_to) {
			charge_requests.enqueue(entity_wants_to_charge_message{ id, charge_to });
		}

		void request_random_move(std::size_t id) {
			wander_requests.enqueue(entity_wants_to_move_randomly_message{ id });
		}

		void process_wandering() {
			using namespace bengine;
			using namespace region;

			wander_requests.process_all([](entity_wants_to_move_randomly_message msg) {
				auto original = entity(msg.entity_id)->component<position_t>();

				position_t pos = *original;
				const int tile_index = mapidx(pos.x, pos.y, pos.z);
				const int direction = rng.roll_dice(1, 6);
				switch (direction) {
				case 1: if (flag(tile_index, CAN_GO_UP)) pos.z++; break;
				case 2: if (flag(tile_index, CAN_GO_DOWN)) pos.z--; break;
				case 3: if (flag(tile_index, CAN_GO_NORTH)) pos.y--; break;
				case 4: if (flag(tile_index, CAN_GO_SOUTH)) pos.y++; break;
				case 5: if (flag(tile_index, CAN_GO_EAST)) pos.x++; break;
				case 6: if (flag(tile_index, CAN_GO_WEST)) pos.x--; break;
				}
				if (!solid(tile_index)) {
					bool can_go = true;
					const int dest = mapidx(pos);

					if (water_level(dest)>2 && water_level(tile_index)<3) can_go = false;

					if (can_go && !(pos == *original)) request_move(entity_wants_to_move_message{ msg.entity_id, pos });
				}
			});
		}

		void process_movement() {
			using namespace bengine;
			using namespace region;
			move_requests.process_all([](entity_wants_to_move_message msg) {
				if (!entity(msg.entity_id)) {
					std::cout << "Oops - move request for entity " << msg.entity_id << ", but entity does not exist\n";
					return;
				}
				auto epos = entity(msg.entity_id)->component<position_t>();
				if (!epos) {
					std::cout << "Oops - move request for entity " << msg.entity_id << ", but entity does not have a position!\n";
					return;
				}
				position_t origin{ epos->x, epos->y, epos->z };
				if (origin == msg.destination) {
					std::cout << "Oops - Moving to same tile, entity " << msg.entity_id << "\n";
					return;
				}

				// Bounds check
				if (msg.destination.x < 1 || msg.destination.x > REGION_WIDTH - 1 || msg.destination.y < 1 ||
					msg.destination.y > REGION_HEIGHT - 1
					|| msg.destination.z < 1 || msg.destination.z > REGION_DEPTH - 1)
					return;

				// Add sliding effect
				auto slide = entity(msg.entity_id)->component<slidemove_t>();
				auto initiative = entity(msg.entity_id)->component<initiative_t>();

				const int dX = msg.destination.x - epos->x;
				const int dY = msg.destination.y - epos->y;
				const int dZ = msg.destination.z - epos->z;

				if (initiative) {
					const float deltaX = (float)dX / (float)initiative->initiative;
					const float deltaY = (float)dY / (float)initiative->initiative;
					const float deltaZ = (float)dZ / (float)initiative->initiative;

					if (!slide && initiative) {
						entity(msg.entity_id)->assign(slidemove_t{ deltaX, deltaY, deltaZ, initiative->initiative });
					}
					else if (slide && initiative) {
						slide->offsetX = deltaX;
						slide->offsetY = deltaY;
						slide->offsetZ = deltaZ;
						slide->lifespan = initiative->initiative;
					}
				}

				// Move
				epos->x = msg.destination.x;
				epos->y = msg.destination.y;
				epos->z = msg.destination.z;
				epos->offsetX = 0.0F - (float)dX;
				epos->offsetY = 0.0F - (float)dY;
				epos->offsetZ = 0.0F - (float)dZ;

				// Do vegetation damage
				const auto idx = mapidx(msg.destination.x, msg.destination.y, msg.destination.z);
				if (veg_type(idx) > 0) {
					// TODO: emit_deferred(vegetation_damage_message{ idx, 1 });
				}

				auto mounted = entity(msg.entity_id)->component<riding_t>();
				if (mounted) {
					auto mount_pos = entity(mounted->riding)->component<position_t>();
					mount_pos->x = epos->x;
					mount_pos->y = epos->y;
					mount_pos->z = epos->z;
					mount_pos->offsetX = epos->offsetX;
					mount_pos->offsetY = epos->offsetY;
					mount_pos->offsetZ = epos->offsetZ;
				}

				move_completions.enqueue(entity_moved_message{ msg.entity_id, origin, msg.destination });
			});
		}

		void process_fleeing() {
			using namespace bengine;
			using namespace region;

			flee_requests.process_all([](entity_wants_to_flee_message msg) {
				auto pos = entity(msg.entity_id)->component<position_t>();
				auto other_pos = entity(msg.flee_from_id)->component<position_t>();

				if (pos->x > other_pos->x && flag(mapidx(pos->x, pos->y, pos->z), CAN_GO_EAST)) {
					move_requests.enqueue(entity_wants_to_move_message{ msg.entity_id, position_t{ pos->x + 1, pos->y, pos->z } });
				}
				else if (pos->x < other_pos->x && flag(mapidx(pos->x, pos->y, pos->z), CAN_GO_WEST)) {
					move_requests.enqueue(entity_wants_to_move_message{ msg.entity_id, position_t{ pos->x - 1, pos->y, pos->z } });
				}
				else if (pos->y < other_pos->y && flag(mapidx(pos->x, pos->y, pos->z), CAN_GO_NORTH)) {
					move_requests.enqueue(entity_wants_to_move_message{ msg.entity_id, position_t{ pos->x, pos->y - 1, pos->z } });
				}
				else if (pos->y > other_pos->y && flag(mapidx(pos->x, pos->y, pos->z), CAN_GO_SOUTH)) {
					move_requests.enqueue(entity_wants_to_move_message{ msg.entity_id, position_t{ pos->x, pos->y + 1, pos->z } });
				}
				else {
					wander_requests.enqueue(entity_wants_to_move_randomly_message{ msg.entity_id });
				}
			});
		}

		void process_charging() {
			using namespace bengine;
			using namespace region;

			charge_requests.process_all([](entity_wants_to_charge_message msg) {
				auto pos = entity(msg.entity_id)->component<position_t>();
				auto other_pos = entity(msg.charge_to_id)->component<position_t>();

				if (pos->x > other_pos->x && flag(mapidx(pos->x, pos->y, pos->z), CAN_GO_WEST)) {
					move_requests.enqueue(entity_wants_to_move_message{ msg.entity_id, position_t{ pos->x - 1, pos->y, pos->z } });
				}
				else if (pos->x < other_pos->x && flag(mapidx(pos->x, pos->y, pos->z), CAN_GO_EAST)) {
					move_requests.enqueue(entity_wants_to_move_message{ msg.entity_id, position_t{ pos->x + 1, pos->y, pos->z } });
				}
				else if (pos->y < other_pos->y && flag(mapidx(pos->x, pos->y, pos->z), CAN_GO_SOUTH)) {
					move_requests.enqueue(entity_wants_to_move_message{ msg.entity_id, position_t{ pos->x, pos->y + 1, pos->z } });
				}
				else if (pos->y > other_pos->y && flag(mapidx(pos->x, pos->y, pos->z), CAN_GO_NORTH)) {
					move_requests.enqueue(entity_wants_to_move_message{ msg.entity_id, position_t{ pos->x, pos->y - 1, pos->z } });
				}
				else {
					wander_requests.enqueue(entity_wants_to_move_randomly_message{ msg.entity_id });
				}
			});
		}

		void update_octree() {
			using namespace bengine;

			if (entity_octree.total_nodes == 0) {
				each<position_t>([](entity_t &e, position_t &pos) {
					entity_octree.add_node(octree_location_t{ static_cast<int>(pos.x), static_cast<int>(pos.y), pos.z, e.id });
				});
			}

			move_completions.process_all([](entity_moved_message msg) {
				octree_location_t start = octree_location_t{ static_cast<int>(msg.origin.x), static_cast<int>(msg.origin.y), msg.origin.z, msg.entity_id };
				octree_location_t end = octree_location_t{ static_cast<int>(msg.destination.x), static_cast<int>(msg.destination.y), msg.destination.z, msg.entity_id };
				entity_octree.remove_node(start);
				entity_octree.add_node(end);

				auto e = entity(msg.entity_id);
				// TODO: Map update notifications
				//if (e && e->component<settler_ai_t>()) {
				//	emit(settler_moved_message{});

				triggers::entry_trigger_firing(msg);
				visibility::on_entity_moved(msg.entity_id);
			});

		}

		void run(const double &duration_ms) {
			process_wandering();
			process_movement();
			process_fleeing();
			update_octree();
		}
	}
}