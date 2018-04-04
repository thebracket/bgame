#include "stdafx.h"
#include "movement_system.hpp"
#include "../../planet/region/region.hpp"
#include "../../global_assets/rng.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../../global_assets/spatial_db.hpp"
#include "trigger_system.hpp"
#include "visibility_system.hpp"
#include "../../render_engine/vox/renderables.hpp"

using namespace tile_flags;

namespace systems {
	namespace movement {

		struct entity_wants_to_move_message {
			entity_wants_to_move_message() = default;
			entity_wants_to_move_message(const int id, const position_t dest) : entity_id(id), destination(dest) { }
			int entity_id;
			position_t destination;
		};

		struct entity_wants_to_move_randomly_message {
			entity_wants_to_move_randomly_message() = default;
			entity_wants_to_move_randomly_message(const int id) : entity_id(id) {}
			int entity_id;
		};

		struct entity_wants_to_flee_message {
			entity_wants_to_flee_message() = default;
			entity_wants_to_flee_message(const int id, const int flee_from) : entity_id(id), flee_from_id(flee_from) {}
			int entity_id;
			int flee_from_id;
		};

		struct entity_wants_to_charge_message {
			entity_wants_to_charge_message() = default;
			entity_wants_to_charge_message(const int id, const int charge_to) : entity_id(id), charge_to_id(charge_to) {}
			int entity_id;
			int charge_to_id;
		};		

		thread_safe_message_queue<entity_wants_to_move_message> move_requests;
		thread_safe_message_queue<entity_wants_to_move_randomly_message> wander_requests;
		thread_safe_message_queue<entity_wants_to_charge_message> charge_requests;
		thread_safe_message_queue<entity_wants_to_flee_message> flee_requests;
		thread_safe_message_queue<entity_moved_message> move_completions;

		void move_to(bengine::entity_t &e, const position_t &pos, const position_t &dest) {
			move_requests.enqueue(entity_wants_to_move_message{ e.id, dest });
		}

		void request_flee(int id, int flee_from) {
			flee_requests.enqueue(entity_wants_to_flee_message{id, flee_from});
		}

		void request_charge(int id, int charge_to) {
			charge_requests.enqueue(entity_wants_to_charge_message{ id, charge_to });
		}

		void request_random_move(int id) {
			wander_requests.enqueue(entity_wants_to_move_randomly_message{ id });
		}

		void process_wandering() {
			using namespace bengine;
			using namespace region;

			wander_requests.process_all([](entity_wants_to_move_randomly_message msg) {
				const auto e = entity(msg.entity_id);
				const auto original = e->component<position_t>();
				if (!original) return;

				auto dest = position_t{};
				dest.x = original->x;
				dest.y = original->y;
				dest.z = original->z;

				const auto tile_index = mapidx(dest.x, dest.y, dest.z);
				const auto direction = rng.roll_dice(1, 10);
				switch (direction) {
				case 1: if (flag(tile_index, CAN_GO_UP)) dest.z++; break;
				case 2: if (flag(tile_index, CAN_GO_DOWN)) dest.z--; break;
				case 3: if (flag(tile_index, CAN_GO_NORTH)) dest.y--; break;
				case 4: if (flag(tile_index, CAN_GO_SOUTH)) dest.y++; break;
				case 5: if (flag(tile_index, CAN_GO_EAST)) dest.x++; break;
				case 6: if (flag(tile_index, CAN_GO_WEST)) dest.x--; break;
				case 7: if (flag(tile_index, CAN_GO_NORTH_EAST)) { dest.y--; dest.x++; } break;
				case 8: if (flag(tile_index, CAN_GO_NORTH_WEST)) { dest.y--; dest.x--; } break;
				case 9: if (flag(tile_index, CAN_GO_SOUTH_EAST)) { dest.y++; dest.x++; } break;
				case 10: if (flag(tile_index, CAN_GO_SOUTH_WEST)) { dest.y++; dest.x--; } break;
				}

				const auto destidx = mapidx(dest);
				if (flag(destidx, CAN_STAND_HERE) && destidx != tile_index) {
					auto can_go = true;

					if (water_level(destidx)>2 && water_level(tile_index)<3) can_go = false;

					if (can_go) move_to(*e, *original, dest);
				}
			});
		}

		void process_movement() {
			using namespace bengine;
			using namespace region;
			move_requests.process_all([](entity_wants_to_move_message msg) {
				if (!entity(msg.entity_id)) {
					glog(log_target::GAME, log_severity::warning, "Oops - move request for entity {0}, but entity does not exist!", msg.entity_id);
					return;
				}
				auto epos = entity(msg.entity_id)->component<position_t>();
				if (!epos) {
					glog(log_target::GAME, log_severity::warning, "Oops - move request for entity {0}, but entity does not have a position!", msg.entity_id);
					return;
				}
				position_t origin{ epos->x, epos->y, epos->z };
				if (origin == msg.destination) {
					glog(log_target::GAME, log_severity::warning, "Oops - Moving to same tile, entity {0}", msg.entity_id);
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

				const auto d_x = msg.destination.x - epos->x;
				const auto d_y = msg.destination.y - epos->y;
				const auto d_z = msg.destination.z - epos->z;
				if (d_x > 0 && d_y > 0) {
					// South-East
					epos->rotation = 315;
				}
				else if (d_x > 0 && d_y < 0) {
					// North-East
					epos->rotation = 225;
				}
				else if (d_x < 0 && d_y < 0 ) {
					// North-West
					epos->rotation = 135;
				}
				else if (d_x < 0 && d_y > 0) {
					// South-West
					epos->rotation = 45;
				}
				else if (d_x > 0) {
					// East
					epos->rotation = 270;
				}
				else if (d_x < 0) {
					// West
					epos->rotation = 90;
				}
				else if (d_y < 0) {
					// North
					epos->rotation = 180;
				}
				else if (d_y > 0) {
					// South
					epos->rotation = 0;
				}

				if (initiative) {
					const auto deltaX = (float)d_x / (float)initiative->initiative;
					const auto deltaY = (float)d_y / (float)initiative->initiative;
					const auto deltaZ = (float)d_z / (float)initiative->initiative;

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
				epos->offset_x = 0.0F - (float)d_x;
				epos->offset_y = 0.0F - (float)d_y;
				epos->offset_z = 0.0F - (float)d_z;

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
					mount_pos->offset_x = epos->offset_x;
					mount_pos->offset_y = epos->offset_y;
					mount_pos->offset_z = epos->offset_z;
					mount_pos->rotation = epos->rotation;
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
				if (!pos || !other_pos) return;

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
				render::models_changed = true;
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