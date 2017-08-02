#pragma once

#include <string>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"

enum item_location_t { INVENTORY, HEAD, TORSO, LEGS, FEET, EQUIP_TOOL, EQUIP_MELEE, EQUIP_RANGED, EQUIP_AMMO, EQUIP_SHIELD };

inline std::string item_loc_name(const item_location_t &loc) {
	switch (loc) {
		case INVENTORY : return std::string("Carried");
		case HEAD : return std::string("Head");
		case TORSO : return std::string("Torso");
		case LEGS : return std::string("Legs");
		case FEET : return std::string("Feet");
		case EQUIP_TOOL : return std::string("Tool");
		case EQUIP_MELEE : return std::string("Melee");
		case EQUIP_RANGED : return std::string("Ranged");
		case EQUIP_AMMO : return std::string("Ammunition");
		case EQUIP_SHIELD : return std::string("Shield");
	}
	return "Unknown";
}

struct item_carried_t {
	item_location_t location = INVENTORY;
	std::size_t carried_by;
	item_carried_t() {}
	item_carried_t(const item_location_t loc, const std::size_t carrier) : location(loc), carried_by(carrier) {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( location, carried_by ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_carried_t>>)