add_library(region
        region.cpp
        )
target_link_libraries(region raws ${ZLIB_LIBS} bengine)
#cotire(region)
