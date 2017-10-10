#include "ai_status_effects.hpp"
#include "../../bengine/ecs.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../../components/item_carried.hpp"
#include "../../components/item.hpp"
#include "../../components/position.hpp"
#include "../../global_assets/spatial_db.hpp"
#include "../../components/item_stored.hpp"
#include "../../global_assets/game_building.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../helpers/inventory_assistant.hpp"
#include "../../components/building.hpp"
#include "../../global_assets/game_camera.hpp"

namespace systems {
	namespace inventory_system {

		using namespace bengine;

		struct inventory_changed_message  {
		};

		struct drop_item_message {
			drop_item_message() {}
			drop_item_message(const std::size_t &ID, const int &X, const int &Y, const int &Z) : id(ID), x(X), y(Y), z(Z) {}
			std::size_t id;
			int x, y, z;
		};

		struct item_claimed_message {
			item_claimed_message() {}
			item_claimed_message(const std::size_t i, const bool c) : claimed(c), id(i) {}
			bool claimed;
			std::size_t id;
		};

		struct pickup_item_message {
			pickup_item_message() {}
			pickup_item_message(const std::size_t &ID, const std::size_t &holder) : id(ID), collector(holder) {}
			pickup_item_message(const std::size_t &ID, const std::size_t &holder, const item_location_t &LOC) : id(ID), collector(holder), loc(LOC) {}
			std::size_t id;
			std::size_t collector;
			item_location_t loc = INVENTORY;
		};

		struct destroy_item_message {
			destroy_item_message() {}
			destroy_item_message(const std::size_t ID) : id(ID) {}
			std::size_t id;
		};

		struct build_request_message {
			int x, y, z;
			buildings::available_building_t building;
			build_request_message() {}
			build_request_message(const int X, const int Y, const int Z, const buildings::available_building_t build) : x(X), y(Y), z(Z), building(build) {}
		};

		thread_safe_message_queue<inventory_changed_message> inventory_changes;
		thread_safe_message_queue<drop_item_message> dropped_items;
		thread_safe_message_queue<item_claimed_message> claimed_items;
		thread_safe_message_queue<pickup_item_message> pickup_items;
		thread_safe_message_queue<destroy_item_message> destroy_items;
		thread_safe_message_queue<build_request_message> building_requests;

		bool dirty = true;

		void run(const double &duration_ms) {
			inventory_changes.process_all([](inventory_changed_message &msg) {
				dirty = true;
			});

			dropped_items.process_all([](drop_item_message &msg) {
				auto E = entity(msg.id);
				if (!E) return;
				claimed_items.enqueue(item_claimed_message{ msg.id, false });

				delete_component<item_carried_t>(msg.id);

				auto item = E->component<item_t>();
				if (!item) return;
				entity(msg.id)->component<item_t>()->claimed = false;
				entity(msg.id)->assign(position_t{ msg.x, msg.y, msg.z });
				entity_octree.add_node(octree_location_t{ msg.x,msg.y,msg.z,msg.id });
				dirty = true;
				// TODO: emit(blocks_changed_message{});
			});

			pickup_items.process_all([](pickup_item_message &msg) {
				if (!entity(msg.id)) return;
				auto pos = entity(msg.id)->component<position_t>();
				if (pos) {
					entity_octree.remove_node(octree_location_t{ static_cast<int>(pos->x), static_cast<int>(pos->y), pos->z,msg.id });
					delete_component<position_t>(msg.id);
				}
				delete_component<item_stored_t>(msg.id);
				entity(msg.id)->assign(item_carried_t{ msg.loc, msg.collector });
				dirty = true;
				// TODO: emit(renderables_changed_message{});
				// TODO: emit(blocks_changed_message{});
			});

			destroy_items.process_all([](destroy_item_message &msg) {
				if (!entity(msg.id)) return;
				auto pos = entity(msg.id)->component<position_t>();
				if (pos) {
					entity_octree.remove_node(octree_location_t{ static_cast<int>(pos->x), static_cast<int>(pos->y), pos->z,msg.id });
					delete_component<position_t>(msg.id);
				}

				delete_entity(msg.id);
				// TODO: emit(blocks_changed_message{});
			});

			claimed_items.process_all([](item_claimed_message &msg) {
				auto e = entity(msg.id);
				if (e) {
					auto item = e->component<item_t>();
					if (item) item->claimed = msg.claimed;
				}
				// TODO: emit(blocks_changed_message{});
			});

			building_requests.process_all([](build_request_message &msg) {
				// Claim components, create the designations
				buildings::available_building_t &building = msg.building;

				// Build designation
				building_designation_t designate;
				designate.x = msg.x;
				designate.y = msg.y;
				designate.z = msg.z;
				designate.name = building.name;
				designate.tag = building.tag;
				designate.components = building.components;
				designate.width = building.width;
				designate.height = building.height;
				//designate.glyphs = building.glyphs;
				//designate.glyphs_ascii = building.glyphs_ascii;

				for (const auto &requested_component : building.components) {
					const std::size_t component_id = inventory::claim_item_by_reaction_input(requested_component);
					std::cout << "Component [" << requested_component.tag << "] #" << component_id << "\n";
					designate.component_ids.push_back(std::make_pair(component_id, false));
				}

				auto building_template = create_entity()
					->assign(position_t{ msg.x, msg.y, msg.z })
					->assign(building_t{ designate.tag, designate.width, designate.height, designate.glyphs,
						designate.glyphs_ascii, false, 0, 10, 10, 0 }); // TODO : Mark the last parameter as the voxel model
				designate.building_entity = building_template->id;
				for (int y = msg.y; y<msg.y + designate.height; ++y) {
					for (int x = msg.x; x < msg.x + designate.width; ++x) {
						entity_octree.add_node(octree_location_t{ x,y,msg.z,building_template->id });
					}
				}

				designations->buildings.push_back(designate);

				int sx = designate.x;
				int sy = designate.y;
				if (designate.width == 3) --sx;
				if (designate.height == 3) --sy;

				for (int x = sx; x < sx + designate.width; ++x) {
					for (int y = sy; y < sy + designate.height; ++y) {
						const auto idx = mapidx(x, y, camera_position->region_z);
						region::set_flag(idx, CONSTRUCTION);
						region::set_veg_type(idx, 0);
						region::calc_render(idx);
					}
				}
			});

			dirty = false;
		}
	}
}