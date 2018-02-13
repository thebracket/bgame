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
#include "ai_tags/ai_mode_idle.hpp"
#include "ai_tags/ai_settler_new_arrival.hpp"
#include "ai_tags/ai_tag_leisure_shift.hpp"
#include "ai_tags/ai_tag_my_turn.hpp"
#include "ai_tags/ai_tag_sleep_shift.hpp"
#include "ai_tags/ai_tag_work_architect.hpp"
#include "ai_tags/ai_tag_work_building.hpp"
#include "ai_tags/ai_tag_work_butcher.hpp"
#include "ai_tags/aI_tag_work_farm_clear.hpp"
#include "ai_tags/aI_tag_work_farm_fertilize.hpp"
#include "ai_tags/aI_tag_work_farm_fixsoil.hpp"
#include "ai_tags/ai_tag_work_farm_water.hpp"
#include "ai_tags/ai_tag_work_farm_weed.hpp"
#include "ai_tags/ai_tag_work_harvest.hpp"
#include "ai_tags/ai_tag_work_hunting.hpp"
#include "ai_tags/ai_tag_work_lumberjack.hpp"
#include "ai_tags/ai_tag_work_mining.hpp"
#include "ai_tags/ai_tag_work_order.hpp"
#include "ai_tags/ai_tag_work_pull_lever.hpp"
#include "ai_tags/ai_tag_work_shift.hpp"
#include "designations.hpp"
#include "position.hpp"

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<position_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<designations_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<farming_designations_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_farm_plant>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_guarding>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_mode_idle_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_settler_new_arrival_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_leisure_shift_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_my_turn_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_sleep_shift_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_architect>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_building>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_butcher>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_farm_clear>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_farm_fertilize>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_farm_fixsoil>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_farm_water>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_farm_weed>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_harvest>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_hunting>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_lumberjack>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_miner>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_order>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_pull_lever>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_shift_t>>)

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

template<class Archive>
void serialize(Archive & archive, ai_mode_idle_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, ai_settler_new_arrival_t &s)
{
	archive(s.turns_since_arrival);
}

template<class Archive>
void serialize(Archive & archive, ai_tag_leisure_shift_t &tag)
{
	// Nothing to save
	//archive( );
}


template<class Archive>
void serialize(Archive & archive, ai_tag_my_turn_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, ai_tag_sleep_shift_t &tag)
{
	// Nothing to save
	//archive( );
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_architect &a)
{
	// Nothing to save
	archive(a.step, a.current_tool);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_building &b)
{
	// Nothing to save
	archive(b.step, b.building_target, b.current_tool);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_butcher &b)
{
	// Nothing to save
	archive(b.step, b.target_corpse);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_farm_clear &f)
{
	// Nothing to save
	archive(f.step, f.tool_id);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_farm_fertilize &f)
{
	// Nothing to save
	archive(f.step, f.tool_id, f.fertilizer_id);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_farm_fixsoil &f)
{
	// Nothing to save
	archive(f.step, f.tool_id, f.soil_id);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_farm_water &f)
{
	// Nothing to save
	archive(f.step, f.tool_id);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_farm_weed &tag)
{
	// Nothing to save
	archive(tag.step, tag.tool_id);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_harvest &f)
{
	// Nothing to save
	archive(f.step, f.tool_id);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_hunting &h)
{
	// Nothing to save
	archive(h.step);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_lumberjack &l)
{
	// Nothing to save
	archive(l.step, l.tool_id, l.target_tree, l.target_x, l.target_y, l.target_z);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_miner &m)
{
	// Nothing to save
	archive(m.step, m.tool_id);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_order & o)
{
	// Nothing to save
	archive(o.step, o.reaction_target, o.current_tool);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_pull_lever &l)
{
	// Nothing to save
	archive(l.step);
}


template<class Archive>
void serialize(Archive & archive, ai_tag_work_shift_t &tag)
{
	// Nothing to save
	//archive( );
}
