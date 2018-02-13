#include "cerealizer.hpp"
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/map.hpp>
#include "../bengine/ecs_impl.hpp"

// Stupid heap size limits
#include "item_tags/item_ammo_t.hpp"
#include "item_tags/item_bone_t.hpp"
#include "item_tags/item_chopping_t.hpp"
#include "item_tags/item_digging_t.hpp"
#include "item_tags/item_drink_t.hpp"
#include "item_tags/item_farming.hpp"
#include "item_tags/item_fertilizer_t.hpp"
#include "item_tags/item_food_t.hpp"
#include "item_tags/item_hide_t.hpp"
#include "item_tags/item_leather_t.hpp"
#include "item_tags/item_melee_t.hpp"
#include "item_tags/item_ranged_t.hpp"
#include "item_tags/item_seed_t.hpp"
#include "item_tags/item_skull_t.hpp"
#include "item_tags/item_spice_t.hpp"
#include "item_tags/item_topsoil_t.hpp"
#include "items/item.hpp"
#include "items/item_carried.hpp"
#include "items/item_creator.hpp"
#include "items/item_quality.hpp"
#include "items/item_stored.hpp"
#include "items/item_wear.hpp"
#include "mining/designated_miner.hpp"
#include "mining/mining_designations.hpp"
#include "helpers/reaction_task_t.hpp"

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_ammo_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_bone_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_chopping_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_digging_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_drink_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_farming_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_fertilizer_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_food_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_hide_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_leather_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_melee_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_ranged_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_seed_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_skull_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_spice_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_topsoil_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_carried_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_creator_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_quality_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_stored_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_wear_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<designated_miner_t>>)
CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<mining_designations_t>>)

template<class Archive>
void serialize(Archive & archive, item_ammo_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_bone_t &b)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_chopping_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_digging_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_drink_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_farming_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_fertilizer_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_food_t &f)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_hide_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_leather_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_melee_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_ranged_t &tag)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, item_seed_t &s)
{
	// Nothing to save
	archive(s.grows_into);
}


template<class Archive>
void serialize(Archive & archive, item_skull_t &tag)
{
	// Nothing to save
	//archive( );
}


template<class Archive>
void serialize(Archive & archive, item_spice_t &tag)
{
	// Nothing to save
	//archive( );
}


template<class Archive>
void serialize(Archive & archive, item_topsoil_t &tag)
{
	// Nothing to save
	//archive( grows_into );
}

template<class Archive>
void serialize(Archive & archive, item_t &i)
{
	archive(i.item_name, i.item_tag, i.type, i.material, i.stack_size, i.clothing_glyph, i.clothing_color, i.clothing_layer); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, item_carried_t &i)
{
	archive(i.location, i.carried_by); // serialize things by passing them to the archive
}

template<class Archive>
void serialize(Archive & archive, item_creator_t &i)
{
	archive(i.creator_id, i.creator_name);
}

template<class Archive>
void serialize(Archive & archive, item_quality_t &i)
{
	// Nothing to save
	archive(i.quality);
}

template<class Archive>
void serialize(Archive & archive, item_stored_t &s)
{
	archive(s.stored_in); // serialize things by passing them to the archive
}


template<class Archive>
void serialize(Archive & archive, item_wear_t &w)
{
	archive(w.wear);
}

template<class Archive>
void serialize(Archive & archive, designated_miner_t &m)
{
	// Nothing to save
	//archive( );
}

template<class Archive>
void serialize(Archive & archive, mining_designations_t &m)
{
	archive(m.mining_targets, m.brush_type, m.brush_size_x, m.brush_size_y, m.mine_mode, m.radius, m.stairs_helper, m.stairs_depth);
}

