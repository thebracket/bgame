#pragma once

#include "../helpers/dijkstra_map.hpp"
#include <memory>

namespace flow_maps
{
	class map_t; // Forward
}

namespace systems {
	namespace distance_map {
		void run(const double &duration_ms);
		
		extern std::unique_ptr<flow_maps::map_t> hunting_map;
		extern std::unique_ptr<flow_maps::map_t> butcher_map;
		extern std::unique_ptr<flow_maps::map_t> bed_map;
		extern std::unique_ptr<flow_maps::map_t> settler_map;
		extern dijkstra::dijkstra_map levers_map;
		extern dijkstra::dijkstra_map reachable_from_cordex;

		void refresh_bed_map();
		void refresh_butcherables_map();
		void refresh_all_distance_maps();
	}
}