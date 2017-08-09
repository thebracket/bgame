add_library(fbo STATIC
        depth_fbo.cpp
        buffertest.cpp
        gbuffer.cpp
        )
cotire(fbo)