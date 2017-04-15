#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "position.hpp"

using namespace rltk;

struct ai_tag_work_lumberjack {

    enum lumberjack_steps { GET_AXE, FIND_TREE, GOTO_TREE, CHOP, DROP_TOOLS };

    ai_tag_work_lumberjack() {}
    lumberjack_steps step = GET_AXE;
    std::size_t current_axe = 0;
    std::size_t target_tree = 0;
    int target_x, target_y, target_z;

    std::shared_ptr<rltk::navigation_path<position_t>> current_path; // Not serialized

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step, current_axe, target_tree, target_x, target_y, target_z );
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<ai_tag_work_lumberjack>>)
