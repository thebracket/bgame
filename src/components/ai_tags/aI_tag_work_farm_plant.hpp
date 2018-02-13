#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../position.hpp"
#include "../../bengine/ecs.hpp"

struct ai_tag_work_farm_plant {

    enum plant_steps { FIND_HOE, FETCH_HOE, FIND_SEED, FETCH_SEED, FIND_TARGET, FETCH_TARGET, PLANT };

	plant_steps step = FIND_HOE;
	std::size_t tool_id = 0;
	std::size_t seed_id = 0;
	position_t farm_position;
	std::shared_ptr<bengine::navigation_path<position_t>> current_path; // Not serialized

    template<class Archive>
    void serialize(Archive & archive)
    {
        // Nothing to save
        archive( step, tool_id, seed_id, farm_position );
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<ai_tag_work_farm_plant>>)
