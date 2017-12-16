#pragma once

#include "../../stdafx.h"
#include "voxel_model.hpp"
#include "../../utils/coroutine_generator.hpp"

namespace vox {
    std::size_t model_index(const std::string tag);
    voxel_model * get_model(const std::size_t &index);

	// Co-routine based loader
	extern std::string vox_to_load;
	extern std::string vox_to_load_tag;
	generator load_vox();
}
