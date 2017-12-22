add_library(shaders
		chunk_shader.cpp
		lightstage_shader.cpp
		voxel_shader.cpp
		voxel_shadow_shader.cpp
)
target_link_libraries(shaders bengine)
