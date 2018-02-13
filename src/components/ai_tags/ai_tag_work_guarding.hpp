#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../position.hpp"
#include "../../bengine/ecs.hpp"
#include "../../bengine/path_finding.hpp"

struct ai_tag_work_guarding {

    enum guard_steps { GOTO_POST, GUARD };

    guard_steps step = GOTO_POST;
    bool has_post = false;
    position_t guard_post;

    std::shared_ptr<bengine::navigation_path<position_t>> current_path; // Not serialized

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step, has_post, guard_post );
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_guarding>>)
