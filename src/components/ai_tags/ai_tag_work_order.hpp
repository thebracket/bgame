#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../position.hpp"
#include "../designations.hpp"

using namespace rltk;

struct ai_tag_work_order {

    enum work_steps { FIND_JOB, SELECT_INPUT, GO_TO_INPUT, COLLECT_INPUT, GO_TO_WORKSHOP, DROP_INPUT, REACT };

    ai_tag_work_order() {}
    work_steps step = FIND_JOB;
    reaction_task_t reaction_target;
    std::size_t current_tool;

    std::shared_ptr<rltk::navigation_path<position_t>> current_path; // Not serialized

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step, reaction_target, current_tool );
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<ai_tag_work_order>>)
