#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

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

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<ai_tag_work_miner>>)
