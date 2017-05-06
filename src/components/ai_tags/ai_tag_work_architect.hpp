#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../position.hpp"

using namespace rltk;

struct ai_tag_work_architect {

    enum architect_steps { GOTO_BLOCK, COLLECT_BLOCK, GOTO_SITE, BUILD };

    ai_tag_work_architect() {}
    architect_steps step = GOTO_BLOCK;
    std::size_t current_tool = 0;

    std::shared_ptr<rltk::navigation_path<position_t>> current_path; // Not serialized

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step, current_tool );
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<ai_tag_work_architect>>)
