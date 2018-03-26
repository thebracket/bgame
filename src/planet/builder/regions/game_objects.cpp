#include "game_objects.hpp"
#include "../../region/region.hpp"
#include "../../../global_assets/farming_designations.hpp"
#include "../../../components/helpers/shift.hpp"
#include "../../../global_assets/game_ecs.hpp"

void build_game_components(const int crash_x, const int crash_y, const int crash_z, const bool &ascii_mode) noexcept {
	calendar_t calendar;
	calendar.defined_shifts.push_back(shift_t{ "Early Shift",{
		WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT }
		});
	calendar.defined_shifts.push_back(shift_t{ "Day Shift",{
		SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT,	WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT }
		});
	calendar.defined_shifts.push_back(shift_t{ "Late Shift",{
		LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, LEISURE_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT, SLEEP_SHIFT,	WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT, WORK_SHIFT }
		});

	bengine::create_entity()
		->assign(world_position_t{region::region_x(), region::region_y(), crash_x, crash_y, crash_z + 1 })
		->assign(std::move(calendar))
		->assign(designations_t{})
		->assign(logger_t{})
		->assign(camera_options_t{ TOP_DOWN, ascii_mode, 12 })
		->assign(mining_designations_t{})
		->assign(farming_designations_t{})
		->assign(building_designations_t{})
		->assign(architecture_designations_t{});
}
