#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../position.hpp"
#include "../designations.hpp"
#include "../../bengine/ecs.hpp"
#include "../../bengine/path_finding.hpp"
#include "../../components/helpers/reaction_task_t.hpp"

struct ai_tag_work_order {

    enum work_steps { FIND_JOB, SELECT_INPUT, GO_TO_INPUT, COLLECT_INPUT, GO_TO_WORKSHOP, DROP_INPUT, REACT };

    ai_tag_work_order() {}
    work_steps step = FIND_JOB;
    reaction_task_t reaction_target;
    std::size_t current_tool;

    std::shared_ptr<bengine::navigation_path<position_t>> current_path; // Not serialized

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step, reaction_target, current_tool );
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_order>>)
