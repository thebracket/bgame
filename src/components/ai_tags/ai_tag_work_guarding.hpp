#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../position.hpp"

using namespace rltk;

struct ai_tag_work_guarding {

    enum guard_steps { GOTO_POST, GUARD };

    ai_tag_work_guarding() {}
    guard_steps step = GOTO_POST;
    bool has_post = false;
    position_t guard_post;

    std::shared_ptr<rltk::navigation_path<position_t>> current_path; // Not serialized

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step, has_post, guard_post );
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<ai_tag_work_guarding>>)
