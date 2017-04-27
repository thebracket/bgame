#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "position.hpp"

using namespace rltk;

struct ai_tag_work_pull_lever {

    enum lever_steps { FIND, PULL };

    ai_tag_work_pull_lever() {}
    lever_steps step = FIND;

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step );
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<ai_tag_work_pull_lever>>)
