#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../position.hpp"

using namespace rltk;

struct ai_tag_work_hunting {

    enum hunting_steps { FIND_TARGET, HUNT };

    ai_tag_work_hunting() {}
    hunting_steps step = FIND_TARGET;

    std::shared_ptr<rltk::navigation_path<position_t>> current_path; // Not serialized

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step );
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<ai_tag_work_hunting>>)
