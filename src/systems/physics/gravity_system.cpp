#include "../../global_assets/game_ecs.hpp"
#include "../helpers/targeted_flow_map.hpp"
#include "../damage/damage_system.hpp"
#include "../../global_assets/rng.hpp"

namespace systems {
	namespace gravity {
		void run(const double &duration_ms) {
			using namespace bengine;

			// Scan to see who shouldn't be falling
			each_without_both<falling_t, flying_t, position_t>([] (entity_t &e, position_t &pos)
			{
				const auto idx = mapidx(pos);
				if (!region::flag(idx, tile_flags::CAN_STAND_HERE))
				{
					e.assign(falling_t{0});
				}
			});

			// Everyone who is falling should fall if they can
			each<falling_t, position_t>([] (entity_t &e, falling_t &f, position_t &pos)
			{
				const auto idx = mapidx(pos);
				if (!region::flag(idx, tile_flags::CAN_STAND_HERE))
				{
					// Fall some more
					pos.z--;
					++f.distance;
				} else
				{
					// We hit the bottom! Ouch.
					if (f.distance > 0) {
						const auto fall_damage = rng.roll_dice(f.distance, 6);
						damage_system::inflict_damage(damage_system::inflict_damage_message{ e.id, fall_damage, "Falling" });
						delete_component<falling_t>(e.id);
					}
				}
			});
		}
	}
}
