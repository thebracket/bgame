#include "bloom_ping_pong.hpp"
#include "../../bengine/gl_include.hpp"
#include <iostream>

namespace render {
    bloom_pingpong_t::bloom_pingpong_t(const int &w, const int &h) {
        width = w;
        height = h;

        gen_buffer1();
        gen_buffer2();
    }

    void bloom_pingpong_t::gen_buffer1() {
        glGenFramebuffers(1, &fbo1_id);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo1_id);

        // color buffer
        glGenTextures(1, &blur_tex1);
        glBindTexture(GL_TEXTURE_2D, blur_tex1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blur_tex1, 0);

        // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
        unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, attachments);

        // create and attach depth buffer (renderbuffer)
        glGenRenderbuffers(1, &rbo1_id);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo1_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo1_id);

        // finally check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer blur1 not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void bloom_pingpong_t::gen_buffer2() {
        glGenFramebuffers(1, &fbo2_id);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo2_id);

        // color buffer
        glGenTextures(1, &blur_tex2);
        glBindTexture(GL_TEXTURE_2D, blur_tex2);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blur_tex2, 0);

        // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
        unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, attachments);

        // create and attach depth buffer (renderbuffer)
        glGenRenderbuffers(1, &rbo2_id);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo2_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo2_id);

        // finally check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer blur2 not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}