add_library(fbo STATIC
        buffertest.cpp
        gbuffer.cpp
        base_lit_buffer.cpp
        )
cotire(fbo)