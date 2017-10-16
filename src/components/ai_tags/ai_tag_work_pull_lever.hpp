#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../position.hpp"
#include "../../bengine/ecs.hpp"

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

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_pull_lever>>)
