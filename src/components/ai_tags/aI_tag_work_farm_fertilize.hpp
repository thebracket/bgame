#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../position.hpp"
#include "../../bengine/ecs.hpp"

struct ai_tag_work_farm_fertilize {

    enum fertilize_steps { FIND_HOE, FETCH_HOE, FIND_FERTILZER, FETCH_FERTILIZER, FIND_TARGET, FETCH_TARGET, FERILIZE };

	fertilize_steps step = FIND_HOE;
	std::size_t tool_id = 0;
	std::size_t fertilizer_id = 0;
	std::shared_ptr<bengine::navigation_path<position_t>> current_path; // Not serialized

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step, tool_id, fertilizer_id );
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_farm_fertilize>>)
