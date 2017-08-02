add_library(region STATIC
        region.cpp
        )
target_link_libraries(region raws)
cotire(region)
