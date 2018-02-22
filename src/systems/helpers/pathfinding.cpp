#include "pathfinding.hpp"
#include "../../planet/constants.hpp"
#include "../../bengine/geometry.hpp"
#include "targeted_flow_map.hpp"
#include <array>
#include <map>
#include <queue>
#include <boost/container/flat_map.hpp>
#include <set>

namespace impl {

	constexpr float MAX_DIRECT_PATH_CHECK = 24.0f;

	std::map<int, std::map<int, navigation_path_t>> cached_paths;

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

	constexpr static int Z_WEIGHT = 10;

	class a_star_t
	{
	public:
		a_star_t(const position_t &start_pos, const position_t &end_pos) noexcept : start_(mapidx(start_pos)), end_(mapidx(end_pos)), end_loc_(end_pos)
		{
			end_x_ = end_pos.x;
			end_y_ = end_pos.y;
			end_z_ = end_pos.z;
			open_list_.emplace_back(node_t{ start_, 0.0f, 0.0f, 0.0f });
		}


		float distance_to_end(const int &idx) const
		{
			const auto[x, y, z] = idxmap(idx);
			return bengine::distance3d(x, y, z*Z_WEIGHT, end_x_, end_y_, end_z_*Z_WEIGHT); // We're weighting zs because stairs tend to not be where you want them
		}

		bool add_successor(const node_t &q, const int &idx)
		{
			auto[x, y, z] = idxmap(idx);
			auto[gx, gy, gz] = idxmap(end_);
			std::cout << "Testing " << idx << " = " << x << "," << y << "," << z << "\n";
			std::cout << "Goal remains: " << end_ << " = " << gx << "," << gy << "," << gz << "\n";
			// Did we reach our goal?
			if (idx == end_)
			{
				std::cout << "It's a goal!\n";
				parents_.insert(std::make_pair(idx, q.idx));
				return true;
			} else
			{
				const auto distance = distance_to_end(idx);
				const node_t s{ idx, distance+1.0f, 1.0f, distance };
				std::cout << "Made a new node, " << idx << ", " << distance << "\n";

				auto should_add = true;

				// If a node with the same possition as successor is in the open list with a lower f, skip add
				for (const auto &e : open_list_)
				{
					if (e.idx == idx && e.f <= s.f)
					{
						should_add = false;
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
					std::cout << "Adding\n";
					open_list_.emplace_back(s);
					//if (parents_.find(idx) != parents_.end()) parents_.erase(idx);
					parents_.insert(std::make_pair(idx, q.idx));
				} else
				{
					std::cout << "Not adding\n";
				}
			}
			return false;
		}

		navigation_path_t found_it(const int &idx) const noexcept
		{
			std::cout << "Goal at " << idx << "\n";
			auto result = navigation_path_t{};
			result.success = true;
			result.destination = end_loc_;

			result.steps.push_front(end_loc_);
			auto current = end_;
			while (current != start_)
			{
				const auto parent = parents_.find(current)->second;
				std::cout << "Parent is " << parent << "\n";
				auto[x, y, z] = idxmap(parent);
				result.steps.push_front(position_t{x,y,z});
				current = parent;
				std::cout << "STEP: " << x << "," << y << "," << z << "\n";
			}

			return result;
		}

		std::shared_ptr<navigation_path_t> search() noexcept
		{
			auto result = std::make_shared<navigation_path_t>();

			while (!open_list_.empty())
			{
				// Pop Q off of the list
				const auto q = open_list_.front();
				open_list_.erase(open_list_.begin());
				std::cout << "Popped a Q! Distance metric is " << q.f << " / " << q.h << "\n";
				auto[x, y, z] = idxmap(q.idx);
				std::cout << "We're considering how to leave " << x << "," << y << "," << z << "\n";

				// Generate successors
				std::vector<int> successors;
				if (region::flag(q.idx, tile_flags::CAN_GO_NORTH)) {
					std::cout << "N-";
					successors.emplace_back(mapidx(x, y - 1, z));
				}
				if (region::flag(q.idx, tile_flags::CAN_GO_SOUTH)) {
					std::cout << "S-";
					successors.emplace_back(mapidx(x, y + 1, z));
				}
				if (region::flag(q.idx, tile_flags::CAN_GO_WEST)) {
					std::cout << "W-";
					successors.emplace_back(mapidx(x - 1, y, z));
				}
				if (region::flag(q.idx, tile_flags::CAN_GO_EAST)) {
					std::cout << "E-";
					successors.emplace_back(mapidx(x + 1, y, z));
				}
				if (region::flag(q.idx, tile_flags::CAN_GO_UP)) {
					std::cout << "U-";
					successors.emplace_back(mapidx(x, y, z + 1));
				}
				if (region::flag(q.idx, tile_flags::CAN_GO_DOWN)) {
					std::cout << "D-";
					successors.emplace_back(mapidx(x, y, z - 1));
				}
				if (region::flag(q.idx, tile_flags::CAN_GO_NORTH_EAST)) successors.emplace_back(mapidx(x + 1, y - 1, z));
				if (region::flag(q.idx, tile_flags::CAN_GO_NORTH_WEST)) successors.emplace_back(mapidx(x - 1, y - 1, z));
				if (region::flag(q.idx, tile_flags::CAN_GO_SOUTH_EAST)) successors.emplace_back(mapidx(x + 1, y + 1, z));
				if (region::flag(q.idx, tile_flags::CAN_GO_SOUTH_WEST)) successors.emplace_back(mapidx(x - 1, y + 1, z));
				std::cout << "\n";

				for (const auto &s : successors)
				{
					if (add_successor(q, s))
					{
						// We found it!
						auto success = found_it(s);
						std::cout << "Steps: " << success.steps.size() << "\n";
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
			return result;
		}

	private:
		int start_;
		int end_;
		int end_x_, end_y_, end_z_;
		std::vector<node_t> open_list_;
		boost::container::flat_map<int, float> closed_list_;
		std::map<int, int> parents_;
		position_t end_loc_;
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
			if (seen_nodes.find(idx) != seen_nodes.end()) {
				result->steps.emplace_back(position_t{ x, y, start.z });
				seen_nodes.insert(idx);
			}
			if (!region::flag(idx, tile_flags::CAN_STAND_HERE)) blocked = true;
		});
		if (!blocked) result->success = true;

		return result;
	}

	static void cache_path(const int &start_idx, const int &end_idx, navigation_path_t path) noexcept
	{
		const auto finder = cached_paths.find(start_idx);
		if (finder == cached_paths.end())
		{
			cached_paths.insert(std::make_pair(start_idx, std::map<int, navigation_path_t>{ {end_idx, path}}));
		}
		else
		{
			finder->second.insert(std::make_pair(end_idx, path));
		}
	}

	static std::shared_ptr<navigation_path_t> find_path(const position_t &start, const position_t &end) noexcept
	{
		// Step 1 - return a cached path if we have one
		const auto start_idx = mapidx(start);
		const auto end_idx = mapidx(end);
		/*
		const auto cache_finder = cached_paths.find(start_idx);
		if (cache_finder != cached_paths.end())
		{
			const auto inner_cache_finder = cache_finder->second.find(end_idx);
			if (inner_cache_finder != cache_finder->second.end())
			{
				return std::make_shared<navigation_path_t>(inner_cache_finder->second);
			}
		}

		// Step 2 - check for the simple straight line option on short, flat paths
		const auto distance = bengine::distance3d(start.x, start.y, start.z, end.x, end.y, end.z);
		if (distance < MAX_DIRECT_PATH_CHECK && start.z == end.z)
		{
			auto result = short_direct_line_optimization(start, end);
			if (result->success) {
				cache_path(start_idx, end_idx, *result);
				return result;
			}
		}*/

		// Step 3 - Try A*
		auto result = a_star(start, end);
		//if (result->success) cache_path(start_idx, end_idx, *result);
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

void invalidate_path_cache() noexcept
{
	impl::cached_paths.clear();
}