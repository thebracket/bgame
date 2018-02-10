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
		extern dijkstra::dijkstra_map levers_map;

		void refresh_bed_map();
		void refresh_hunting_map();
		void refresh_architecture_map();
		void refresh_butcherables_map();
		void refresh_all_distance_maps();
	}
}