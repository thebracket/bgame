#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../position.hpp"

using namespace rltk;

struct ai_tag_work_butcher {

    enum butcher_steps { FIND_CORPSE, GO_TO_CORPSE, COLLECT_CORPSE, GO_TO_SHOP, CHOP };

    ai_tag_work_butcher() {}
    butcher_steps step = FIND_CORPSE;
    std::size_t target_corpse = 0;

    std::shared_ptr<rltk::navigation_path<position_t>> current_path; // Not serialized

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step, target_corpse );
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<ai_tag_work_butcher>>)
