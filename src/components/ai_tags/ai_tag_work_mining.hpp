#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../../bengine/ecs.hpp"

struct ai_tag_work_miner {

    enum mining_steps { GET_PICK, GOTO_SITE, DIG, DROP_TOOLS };

    ai_tag_work_miner() {}
    mining_steps step = GET_PICK;
    std::size_t current_pick = 0;

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step, current_pick );
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_miner>>)
