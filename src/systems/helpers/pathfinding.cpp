#include "stdafx.h"
#include "pathfinding.hpp"
#include "../../planet/constants.hpp"
#include "../../bengine/geometry.hpp"
#include "targeted_flow_map.hpp"
#include <array>
#include <queue>
#include <boost/container/flat_map.hpp>
#include <set>

namespace impl {

	constexpr static float MAX_DIRECT_PATH_CHECK = 24.0f;
	constexpr static int Z_WEIGHT = 10;
	constexpr static int MAX_ASTAR_STEPS = 500;

	struct node_t
	{
		int idx = 0;
		float f = 0.0f;
		float g = 0.0f; // Movement cost to this square
		float h = 0.0f; // Estimated cost from here to the final destination

		bool operator<(const node_t &other) const
		{
			return f < other.f; // We're inverting the priority queue!
		}
	};


	class a_star_t
	{
	public:
		a_star_t(const position_t &start_pos, const position_t &end_pos) noexcept : start_(mapidx(start_pos)), end_(mapidx(end_pos)), end_loc_(end_pos)
		{
			end_x_ = end_pos.x;
			end_y_ = end_pos.y;
			end_z_ = end_pos.z;
			open_list_.reserve(500);
			closed_list_.reserve(300);
			parents_.reserve(600);

			open_list_.emplace_back(node_t{ start_, 0.0f, 0.0f, 0.0f });
		}

		float distance_to_end(const int &idx) const
		{
			const auto[x, y, z] = idxmap(idx);
			return bengine::distance3d_squared(x, y, z*Z_WEIGHT, end_x_, end_y_, end_z_*Z_WEIGHT); // We're weighting zs because stairs tend to not be where you want them
		}

		bool add_successor(const node_t &q, const int &idx)
		{
			// Did we reach our goal?
			if (idx == end_)
			{
				parents_.insert(std::make_pair(idx, q.idx));
				return true;
			} else
			{
				const auto distance = distance_to_end(idx);
				const node_t s{ idx, distance+1.0f, 1.0f, distance };

				auto should_add = true;

				// If a node with the same possition as successor is in the open list with a lower f, skip add
				for (const auto &e : open_list_)
				{
					if (e.f <= s.f) {
						if (e.idx == idx)
						{
							should_add = false;
							goto skipper;
						}
					} else
					{
						goto skipper;
					}
				}

				skipper:

				// If a node with the same position as successor is in the closed list, with a lower f, skip add
				{
					const auto closed_finder = closed_list_.find(idx);
					if (closed_finder != closed_list_.end())
					{
						if (closed_finder->second <= s.f) should_add = false;
					}
				}

				if (should_add) {
					open_list_.emplace_back(s);
					//if (parents_.find(idx) != parents_.end()) parents_.erase(idx);
					parents_.insert(std::make_pair(idx, q.idx));
				}
			}
			return false;
		}

		navigation_path_t found_it(const int &idx) const noexcept
		{
			auto result = navigation_path_t{};
			result.success = true;
			result.destination = end_loc_;

			result.steps.push_front(end_loc_);
			auto current = end_;
			while (current != start_)
			{
				const auto parent = parents_.find(current)->second;
				auto[x, y, z] = idxmap(parent);
				if (parent != start_) result.steps.push_front(position_t{x,y,z});
				current = parent;
			}

			return result;
		}

		std::shared_ptr<navigation_path_t> search() noexcept
		{
			auto result = std::make_shared<navigation_path_t>();

			while (!open_list_.empty() && step_counter_ < MAX_ASTAR_STEPS)
			{
				++step_counter_;

				// Pop Q off of the list
				const auto q = open_list_.front();
				open_list_.erase(open_list_.begin());
				auto[x, y, z] = idxmap(q.idx);

				// Generate successors
				std::vector<int> successors;
				const auto flags = region::get_flag_reference(q.idx);
				if (flags.test(tile_flags::CAN_GO_NORTH)) successors.emplace_back(mapidx(x, y - 1, z));
				if (flags.test(tile_flags::CAN_GO_SOUTH)) successors.emplace_back(mapidx(x, y + 1, z));
				if (flags.test(tile_flags::CAN_GO_WEST)) successors.emplace_back(mapidx(x - 1, y, z));
				if (flags.test(tile_flags::CAN_GO_EAST)) successors.emplace_back(mapidx(x + 1, y, z));
				if (flags.test(tile_flags::CAN_GO_UP)) successors.emplace_back(mapidx(x, y, z + 1));
				if (flags.test(tile_flags::CAN_GO_DOWN)) successors.emplace_back(mapidx(x, y, z - 1));
				if (flags.test(tile_flags::CAN_GO_NORTH_EAST)) successors.emplace_back(mapidx(x + 1, y - 1, z));
				if (flags.test(tile_flags::CAN_GO_NORTH_WEST)) successors.emplace_back(mapidx(x - 1, y - 1, z));
				if (flags.test(tile_flags::CAN_GO_SOUTH_EAST)) successors.emplace_back(mapidx(x + 1, y + 1, z));
				if (flags.test(tile_flags::CAN_GO_SOUTH_WEST)) successors.emplace_back(mapidx(x - 1, y + 1, z));

				for (const auto &s : successors)
				{
					if (add_successor(q, s))
					{
						// We found it!
						const auto success = found_it(s);
						result->success = success.success;
						result->steps = success.steps;
						result->destination = success.destination;
						goto BAILOUT;
					}
				}

				if (closed_list_.find(q.idx) != closed_list_.end()) closed_list_.erase(q.idx);
				closed_list_.insert(std::make_pair(q.idx, q.f));
				std::sort(open_list_.begin(), open_list_.end());
			}
			result->success = false;
		BAILOUT:
			/*
			std::cout << "Usage stats from completed search:\n";
			std::cout << "Open list size: " << open_list_.size() << "\n";
			std::cout << "Closed list size: " << closed_list_.size() << "\n";
			std::cout << "Parents size: " << parents_.size() << "\n";
			*/
			return result;
		}

	private:
		int start_;
		int end_;
		int end_x_, end_y_, end_z_;
		std::vector<node_t> open_list_;
		boost::container::flat_map<int, float> closed_list_;
		boost::container::flat_map<int, int> parents_;
		position_t end_loc_;
		int step_counter_ = 0;
	};

	static std::shared_ptr<navigation_path_t> a_star(const position_t &start, const position_t &end) noexcept
	{
		a_star_t searcher(start, end);
		return searcher.search();
	}

	static std::shared_ptr<navigation_path_t> short_direct_line_optimization(const position_t &start, const position_t &end) noexcept
	{
		auto result = std::make_shared<navigation_path_t>();
		auto blocked = false;
		std::set<int> seen_nodes;
		bengine::line_func(start.x, start.y, end.x, end.y, [&blocked, &start, &result, &seen_nodes](const int &x, const int &y)
		{
			const auto idx = mapidx(x, y, start.z);
			if (seen_nodes.find(idx) == seen_nodes.end()) {
				if (idx != mapidx(start)) result->steps.emplace_back(position_t{ x, y, start.z });
				seen_nodes.insert(idx);
			}
			if (!region::flag(idx, tile_flags::CAN_STAND_HERE)) blocked = true;
		});
		if (!blocked) result->success = true;

		return result;
	}
	
	static std::shared_ptr<navigation_path_t> find_path(const position_t &start, const position_t &end) noexcept
	{
		// Step 2 - check for the simple straight line option on short, flat paths
		/*const auto distance = bengine::distance3d(start.x, start.y, start.z, end.x, end.y, end.z);
		if (distance < MAX_DIRECT_PATH_CHECK && start.z == end.z)
		{
			auto result = short_direct_line_optimization(start, end);
			if (result->success) {
				return result;
			}
		}*/

		// Step 3 - Try A*
		auto result = a_star(start, end);
		return result;
	}

}

std::shared_ptr<navigation_path_t> find_path(const position_t &start, const position_t &end, const bool find_adjacent, const std::size_t civ) noexcept
{
	// Step 1 - check clipping on both ends; the path auto-fails fast if there is an out-of-bounds issue.
	if (start.x < 1 || start.x > REGION_WIDTH - 1 || start.y < 1 || start.y > REGION_HEIGHT - 1 || start.z < 1 || start.z > REGION_DEPTH + 1
		|| end.x < 1 || end.x > REGION_WIDTH - 1 || end.y < 1 || end.y > REGION_HEIGHT - 1 || end.z < 1 || end.z > REGION_DEPTH + 1) 
	{
		auto fail = std::make_shared<navigation_path_t>();
		fail->success = false;
		return fail;
	}

	if (start == end)
	{
		auto stay = std::make_shared<navigation_path_t>();
		stay->success = true;
		stay->destination = end;
		stay->steps.push_front(start);
		return stay;
	}

	constexpr auto num_paths = 10;

	auto result = impl::find_path(start, end);
	if (find_adjacent && !result->success) {
		std::array<position_t, num_paths> candidates{
			position_t{ end.x - 1, end.y, end.z },
			position_t{ end.x + 1, end.y, end.z },
			position_t{ end.x, end.y - 1, end.z },
			position_t{ end.x, end.y + 1, end.z },
			position_t{ end.x + 1, end.y + 1, end.z },
			position_t{ end.x - 1, end.y + 1, end.z },
			position_t{ end.x + 1, end.y - 1, end.z },
			position_t{ end.x - 1, end.y - 1, end.z },
			position_t{ end.x, end.y, end.z - 1 },
			position_t{ end.x, end.y, end.z + 1 },
		};
		for (const auto &candidate : candidates)
		{
			result.reset();
			result = impl::find_path(start, candidate);
			if (result->success) return result;
		}
	}
	return result;
}
