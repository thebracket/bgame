#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../position.hpp"
#include "../../bengine/ecs.hpp"

struct ai_tag_work_architect {

    enum architect_steps { GOTO_BLOCK, COLLECT_BLOCK, GOTO_SITE, BUILD };

    ai_tag_work_architect() {}
    architect_steps step = GOTO_BLOCK;
    std::size_t current_tool = 0;

    std::shared_ptr<bengine::navigation_path<position_t>> current_path; // Not serialized

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step, current_tool );
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_architect>>)
