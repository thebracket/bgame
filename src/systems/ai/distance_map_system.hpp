#pragma once

#include "../helpers/dijkstra_map.hpp"

namespace systems {
	namespace distance_map {
		void run(const double &duration_ms);

		extern dijkstra::dijkstra_map huntables_map;
		extern dijkstra::dijkstra_map butcherables_map;
		extern dijkstra::dijkstra_map bed_map;
		extern dijkstra::dijkstra_map settler_map;
		extern dijkstra::dijkstra_map architecure_map;
		extern dijkstra::dijkstra_map blocks_map;
		extern dijkstra::dijkstra_map levers_map;
		extern dijkstra::dijkstra_map axe_map;
		extern dijkstra::dijkstra_map pick_map;
		extern dijkstra::dijkstra_map harvest_map;

		void refresh_bed_map();
	}
}