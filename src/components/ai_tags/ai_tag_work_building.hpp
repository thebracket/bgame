#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../position.hpp"
#include "../designations.hpp"

using namespace rltk;

struct ai_tag_work_building {

    enum building_steps { SELECT_BUILDING, SELECT_COMPONENT, GO_TO_COMPONENT, COLLECT_COMPONENT, GO_TO_BUILDING, DROP_COMPONENT, ASSEMBLE };

    ai_tag_work_building() {}
    building_steps step = SELECT_BUILDING;
    building_designation_t building_target;
    std::size_t current_tool;

    std::shared_ptr<rltk::navigation_path<position_t>> current_path; // Not serialized

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step, building_target, current_tool );
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<ai_tag_work_building>>)
