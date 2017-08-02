add_library(region STATIC
        region.cpp
        )
target_link_libraries(region raws ${ZLIB_LIBS} bengine)
cotire(region)
