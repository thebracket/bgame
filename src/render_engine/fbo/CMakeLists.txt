add_library(fbo STATIC
        depth_fbo.cpp
        buffertest.cpp
        gbuffer.cpp
        base_lit_buffer.cpp
        )
cotire(fbo)