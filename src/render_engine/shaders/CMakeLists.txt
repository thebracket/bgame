add_library(shaders
	chunk_shader.cpp
	voxel_shader.cpp
)
target_link_libraries(shaders bengine)
