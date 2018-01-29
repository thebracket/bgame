#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../../bengine/ecs.hpp"

struct ai_tag_work_miner {

    enum mining_steps { GET_PICK, FETCH_PICK, GOTO_SITE, DIG };

    ai_tag_work_miner() {}
    mining_steps step = GET_PICK;
	std::size_t pick_id = 0;

	std::shared_ptr<bengine::navigation_path<position_t>> current_path; // Not serialized

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step, pick_id );
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_miner>>)
