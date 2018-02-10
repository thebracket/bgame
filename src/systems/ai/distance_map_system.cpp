#include "distance_map_system.hpp"
#include "../../components/position.hpp"
#include "../../planet/region/region.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../components/grazer_ai.hpp"
#include "../../components/corpse_harvestable.hpp"
#include "../../components/buildings/construct_provides_sleep.hpp"
#include "../../components/claimed_t.hpp"
#include "../../components/settler_ai.hpp"

namespace systems {
	namespace distance_map {

		using namespace dijkstra;

		dijkstra_map huntables_map;
		dijkstra_map butcherables_map;
		dijkstra_map bed_map;
		dijkstra_map settler_map;
		dijkstra_map levers_map;
		bool dijkstra_debug = false;

		static bool huntables_dirty = true;
		static bool butcherables_dirty = true;
		static bool beds_dirty = true;
		static bool settlers_dirty = true;
		static bool levers_dirty = true;

		using namespace bengine;

		void refresh_bed_map() {
			beds_dirty = true;
		}

		void refresh_hunting_map() {
			huntables_dirty = true;
		}

		void refresh_butcherables_map() {
			butcherables_dirty = true;
		}

		void refresh_all_distance_maps() {
			beds_dirty = true;
			huntables_dirty = true;
			butcherables_dirty = true;
		}

		void update_hunting_map() {
			std::vector<int> huntables;
			each<grazer_ai, position_t>([&huntables](entity_t &e, grazer_ai &ai, position_t &pos) {
				huntables.emplace_back(mapidx(pos));
			});
			huntables_map.update(huntables);
		}

		void update_butcher_map() {
			std::vector<int> butcherables;
			each<corpse_harvestable, position_t>([&butcherables](entity_t &e, corpse_harvestable &corpse, position_t &pos) {
				butcherables.emplace_back(mapidx(pos));
			});
			butcherables_map.update(butcherables);
		}

		void update_bed_map() {
			std::vector<int> beds;
			each_without<claimed_t, construct_provides_sleep_t, position_t>([&beds](entity_t &e, construct_provides_sleep_t &bed, position_t &pos) {
				beds.emplace_back(mapidx(pos));
			});
			bed_map.update(beds);
		}

		void update_settler_map() {
			std::vector<int> settlers;
			each<settler_ai_t, position_t>([&settlers](entity_t &e, settler_ai_t &settler, position_t &pos) {
				settlers.emplace_back(mapidx(pos));
			});
			settler_map.update(settlers);
		}

		void update_levers_map() {
			std::vector<int> targets;
			for (auto it = designations->levers_to_pull.begin(); it != designations->levers_to_pull.end(); ++it) {
				auto lever_id = *it;
				auto lever_entity = entity(lever_id);
				if (!lever_entity) break;
				auto lever_pos = lever_entity->component<position_t>();
				if (!lever_pos) break;
				targets.emplace_back(mapidx(*lever_pos));
			}
			levers_map.update(targets);
		}

		void run(const double &duration_ms) {
			if (huntables_dirty) {
				update_hunting_map();
				huntables_dirty = false;
			}

			if (butcherables_dirty) {
				update_butcher_map();
				butcherables_dirty = false;
			}

			if (beds_dirty) {
				update_bed_map();
				beds_dirty = false;
			}

			if (settlers_dirty) {
				update_settler_map();
				settlers_dirty = false;
			}

			if (levers_dirty) {
				update_levers_map();
				levers_dirty = false;
			}
		}
	}
}