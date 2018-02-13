#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../position.hpp"
#include "../../bengine/ecs.hpp"

struct ai_tag_work_butcher {

    enum butcher_steps { FIND_CORPSE, GO_TO_CORPSE, COLLECT_CORPSE, GO_TO_SHOP, CHOP };

    butcher_steps step = FIND_CORPSE;
    std::size_t target_corpse = 0;

    std::shared_ptr<bengine::navigation_path<position_t>> current_path; // Not serialized

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step, target_corpse );
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_butcher>>)
