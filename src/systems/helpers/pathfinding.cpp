#include "pathfinding.hpp"
#include "../../planet/region/region.hpp"
#include "../../bengine/geometry.hpp"
#include "../../bengine/path_finding.hpp"

using namespace region;
using namespace tile_flags;

std::size_t civ_id = 0;

struct navigator_t {
	inline static void test_direction(const position_t &pos, const position_t &dest, std::vector<position_t> &successors) {
		bool can_go = false;
		const int idx = mapidx(pos);
		if (dest.x > pos.x && flag(idx, CAN_GO_EAST)) {
			can_go = true;
		}
		else if (dest.x < pos.x && flag(idx, CAN_GO_WEST)) {
			can_go = true;
		}
		else if (dest.y < pos.y && flag(idx, CAN_GO_NORTH)) {
			can_go = true;
		}
		else if (dest.y > pos.y && flag(idx, CAN_GO_SOUTH)) {
			can_go = true;
		}
		else if (dest.z > pos.z && flag(idx, CAN_GO_UP)) {
			can_go = true;
		}
		else if (dest.z < pos.z && flag(idx, CAN_GO_DOWN)) {
			can_go = true;
		}

		if (can_go) {
			const int destidx = mapidx(dest);
			// Check for water level
			if (water_level(destidx) > 3) {
				if (water_level(idx) < 4) can_go = false;
			}

			if (can_go) {
				successors.push_back(dest);
			}
		}
	}

	static float get_distance_estimate(position_t &pos, position_t &goal) {
		const float d = bengine::distance3d(pos.x, pos.y, pos.z, goal.x, goal.y, goal.z);
		return d;
	}

	static bool is_goal(position_t &pos, position_t &goal) {
		return pos == goal;
	}

	// This is where we calculate where you can go from a given tile. In this case, we check
	// all 8 directions, and if the destination is walkable return it as an option.
	static bool get_successors(position_t pos, std::vector<position_t> &successors) {
		test_direction(pos, position_t{ pos.x, pos.y - 1, pos.z }, successors);
		test_direction(pos, position_t{ pos.x, pos.y + 1, pos.z }, successors);
		test_direction(pos, position_t{ pos.x + 1, pos.y, pos.z }, successors);
		test_direction(pos, position_t{ pos.x - 1, pos.y, pos.z }, successors);
		test_direction(pos, position_t{ pos.x, pos.y, pos.z - 1 }, successors);
		test_direction(pos, position_t{ pos.x, pos.y, pos.z + 1 }, successors);

		return true;
	}

	// This function lets you set a cost on a tile transition. For now, we'll always use a cost of 1.0.
	static float get_cost(position_t &position, position_t &successor) {
		return 1.0f;
	}

	// This is a simple comparison to determine if two locations are the same. It just passes
	// through to the location_t's equality operator in this instance (we didn't do that automatically)
	// because there are times you might want to behave differently.
	static bool is_same_state(position_t &lhs, position_t &rhs) {
		return lhs == rhs;
	}
};

std::shared_ptr<bengine::navigation_path<position_t>> find_path(const position_t &start, const position_t &end, const bool find_adjacent, const std::size_t civ) {
	if (start.x < 1 || start.x > REGION_WIDTH - 1 || start.y < 1 || start.y > REGION_HEIGHT - 1 || start.z < 1 || start.z > REGION_DEPTH + 1
		|| end.x < 1 || end.x > REGION_WIDTH - 1 || end.y < 1 || end.y > REGION_HEIGHT - 1 || end.z < 1 || end.z > REGION_DEPTH + 1) {
		std::shared_ptr<bengine::navigation_path<position_t>> fail = std::make_shared<bengine::navigation_path<position_t>>();
		fail->success = false;
		return fail;
	}

	civ_id = civ;

	auto result = bengine::find_path<position_t, navigator_t>(start, end);
	if (find_adjacent && result->success == false) {
		std::array<position_t, 6> candidates{
			position_t{ end.x - 1, end.y, end.z },
			position_t{ end.x + 1, end.y, end.z },
			position_t{ end.x, end.y - 1, end.z },
			position_t{ end.x, end.y + 1, end.z },
			position_t{ end.x, end.y, end.z - 1 },
			position_t{ end.x, end.y, end.z + 1 }
		};
		for (int i = 0; i<4; ++i) {
			result.reset();
			result = bengine::find_path<position_t, navigator_t>(start, candidates[i]);
			if (result->success) return result;
		}
	}
	return result;
}
