#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../position.hpp"
#include "../../bengine/ecs.hpp"
#include "../../bengine/path_finding.hpp"

struct ai_tag_work_lumberjack {

    enum lumberjack_steps { GET_AXE, FETCH_AXE, FIND_TREE, GOTO_TREE, CHOP };

    ai_tag_work_lumberjack() {}
    lumberjack_steps step = GET_AXE;
    std::size_t axe_id = 0;
    std::size_t target_tree = 0;
    int target_x, target_y, target_z;

    std::shared_ptr<bengine::navigation_path<position_t>> current_path; // Not serialized

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step, axe_id, target_tree, target_x, target_y, target_z );
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_lumberjack>>)

