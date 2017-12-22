add_library(vox
        voxreader.cpp
        voxel_model.cpp
        renderables.cpp)
target_link_libraries(vox helpers )
cotire (vox)