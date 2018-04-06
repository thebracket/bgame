#include "stdafx.h"
#include "gravity_system.hpp"
#include "../helpers/targeted_flow_map.hpp"
#include "../damage/damage_system.hpp"
#include "../../global_assets/rng.hpp"
#include "../../raws/raws.hpp"
#include "raws/materials.hpp"
#include "topology_system.hpp"

namespace systems {
	namespace gravity {

		std::array<bool, REGION_TILES_COUNT> supported;
		static std::vector<int> open_list;
		static std::array<bool, REGION_TILES_COUNT> considered;
		static bool first_run = true;
		bool tile_removed = true;

		static void check_if_new(const int &idx)
		{
			if (!considered[idx]) {
				open_list.emplace_back(idx);
			}
		}

		static void check_for_collapse()
		{
			if (first_run)
			{
				open_list.reserve(5000000);
				first_run = false;
			}

			std::fill(supported.begin(), supported.end(), false);
			std::fill(considered.begin(), considered.end(), false);
			open_list.clear();

			const auto tiletypes = region::get_tile_types_array();

			open_list.emplace_back(mapidx(0, 0, 0));
			while (!open_list.empty())
			{
				const auto idx = open_list.back();
				open_list.pop_back();

				if (!considered[idx])
				{
					considered[idx] = true;
					supported[idx] = true;
					auto [x, y, z] = idxmap(idx);
					const auto tt = tiletypes->operator[](idx);
					//std::cout << +tt << "\n";
					if (tt != tile_type::OPEN_SPACE && tt != tile_type::FLOOR)
					{
						if (x > 0) check_if_new(idx - 1);
						if (x < REGION_WIDTH - 1) check_if_new(idx + 1);
						if (y > 0) check_if_new(idx - REGION_WIDTH);
						if (y < REGION_HEIGHT - 1) check_if_new(idx + REGION_WIDTH);
						if (z > 1) check_if_new(idx - (REGION_WIDTH * REGION_HEIGHT));
						if (z < REGION_DEPTH - 1) check_if_new(idx + (REGION_WIDTH * REGION_HEIGHT));
						if (z < REGION_DEPTH - 1 && x > 0) check_if_new(idx + (REGION_WIDTH * REGION_HEIGHT) - 1);
						if (z < REGION_DEPTH - 1 && x < REGION_WIDTH-1) check_if_new(idx + (REGION_WIDTH * REGION_HEIGHT) + 1);
						if (z < REGION_DEPTH - 1 && y > 0) check_if_new(idx + (REGION_WIDTH * REGION_HEIGHT) - REGION_WIDTH);
						if (z < REGION_DEPTH - 1 && y < REGION_HEIGHT-1) check_if_new(idx + (REGION_WIDTH * REGION_HEIGHT) + REGION_WIDTH);
					} else if (tt == tile_type::FLOOR)
					{
						if (x > 0) check_if_new(idx - 1);
						if (x < REGION_WIDTH - 1) check_if_new(idx + 1);
						if (y > 0) check_if_new(idx - REGION_WIDTH);
						if (y < REGION_HEIGHT - 1) check_if_new(idx + REGION_WIDTH);
					}
				}
				//std::cout << "Open list size: " << open_list.size() << "\n";
			}

			std::vector<int> collapses;
			for (auto idx=0; idx<REGION_TILES_COUNT; ++idx)
			{
				if (tiletypes->operator[](idx) != tile_type::OPEN_SPACE && tiletypes->operator[](idx) != tile_type::TREE_TRUNK && tiletypes->operator[](idx) != tile_type::TREE_LEAF && !supported[idx])
				{
					collapses.emplace_back(idx);
				}
			}
			std::cout << collapses.size() << " detected\n";

			for (const auto &idx : collapses)
			{
				auto[tx, ty, tz] = idxmap(idx);
				if (tiletypes->operator[](idx) == tile_type::TREE_TRUNK || tiletypes->operator[](idx) == tile_type::TREE_LEAF)
				{
					spawn_item_on_ground(tx, ty, tz, "wood_log", get_material_by_tag("wood"), 3, 100, 0, "");
				} else
				{
					topology::spawn_mining_result(topology::perform_mining_message(idx, 0, tx, ty, tz));
				}
				region::make_open_space(idx);
				std::cout << "Collapse occurred at " << tx << ", " << ty << ", " << tz << "\n";
			}
		}

		static void start_falling()
		{
			using namespace bengine;

			// Scan to see who shouldn't be falling
			each_without_both<falling_t, flying_t, position_t>([](entity_t &e, position_t &pos)
			{
				const auto idx = mapidx(pos);
				if (!region::flag(idx, tile_flags::CAN_STAND_HERE) && region::tile_type(idx) != tile_type::CLOSED_DOOR)
				{
					const auto building = e.component<building_t>();
					if (building)
					{
						// We need to desconstruct it
						for (const auto &component : building->built_with)
						{
							// tag, material
							spawn_item_on_ground(pos.x, pos.y, pos.z, component.first, component.second);
						}
						delete_entity(e.id);
					}
					else {
						e.assign(falling_t{ 0 });
						//std::cout << e.id << " is now falling.\n";
					}
				}
			});
		}

		static void fall()
		{
			using namespace bengine;

			// Everyone who is falling should fall if they can
			each<falling_t, position_t>([](entity_t &e, falling_t &f, position_t &pos)
			{
				//std::cout << e.id << " is falling.\n";
				const auto idx = mapidx(pos);
				if (idx > 0 && idx < REGION_TILES_COUNT) {
					if (!region::flag(idx, tile_flags::CAN_STAND_HERE))
					{
						// Fall some more
						pos.z--;
						++f.distance;
					}
					else
					{
						// We hit the bottom! Ouch.
						if (f.distance > 0) {
							const auto fall_damage = rng.roll_dice(f.distance, 6);
							damage_system::inflict_damage(damage_system::inflict_damage_message{ e.id, fall_damage, "Falling" });
							delete_component<falling_t>(e.id);
							std::cout << e.id << " hit the ground for " << fall_damage << " points of damage.\n";
						}
					}
				}
			});
		}

		void tile_was_removed()
		{
			tile_removed = true;
		}

		void run(const double &duration_ms) {
			if (tile_removed) {
				check_for_collapse();
				tile_removed = false;
			}
			start_falling();
			fall();
		}
	}
}
