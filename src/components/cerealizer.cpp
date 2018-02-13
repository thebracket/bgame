#include "cerealizer.hpp"
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/map.hpp>
#include "../bengine/ecs_impl.hpp"

// All components; yes, this a mess - but the compilation speed-up is worth it!
#include "farming/farm_designs.hpp"
#include "ai_tags/aI_tag_work_farm_plant.hpp"
#include "ai_tags/ai_tag_work_guarding.hpp"
#include "designations.hpp"
#include "position.hpp"

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<position_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<designations_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<farming_designations_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_farm_plant>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_guarding>>)

template<class Archive>
void serialize(Archive & archive, position_t &p)
{
	archive(p.x, p.y, p.z, p.offset_x, p.offset_y, p.offset_z, p.rotation); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, designations_t &d)
{
	archive(d.chopping, d.guard_points, d.deconstructions,
		d.levers_to_pull, d.current_power, d.current_cash, d.standing_order_idle_move, d.standing_order_wildlife_treatment,
		d.standing_order_upgrade); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, farm_cycle_t &f)
{
	archive(f.state, f.seed_type, f.days_since_weeded, f.days_since_watered, f.fertilized, f.seed_id);
}

template<class Archive>
void serialize(Archive & archive, farming_designations_t &f)
{
	archive(f.harvest, f.farms);
}

template<class Archive>
void serialize(Archive & archive, ai_tag_work_farm_plant &f)
{
	// Nothing to save
	archive(f.step, f.tool_id, f.seed_id, f.farm_position);
}

template<class Archive>
void serialize(Archive & archive, ai_tag_work_guarding &g)
{
	// Nothing to save
	archive(g.step, g.has_post, g.guard_post);
}
