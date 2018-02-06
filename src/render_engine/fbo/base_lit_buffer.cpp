#include "base_lit_buffer.hpp"
#include "../../bengine/gl_include.hpp"
#include <iostream>

namespace render {
    base_lit_buffer_t::base_lit_buffer_t(const int &w, const int &h) {
        width = w;
        height = h;

        glGenFramebuffers(1, &fbo_id);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);

        // color buffer
        glGenTextures(1, &color_tex);
        glBindTexture(GL_TEXTURE_2D, color_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_tex, 0);

        // overbright buffer
        glGenTextures(1, &bright_tex);
        glBindTexture(GL_TEXTURE_2D, bright_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bright_tex, 0);

		// shiny buffer
		glGenTextures(1, &shiny_tex);
		glBindTexture(GL_TEXTURE_2D, shiny_tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, shiny_tex, 0);

        // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
        unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, attachments);

        // create and attach depth buffer (renderbuffer)
        glGenRenderbuffers(1, &rbo_id);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_id);

        // finally check if framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer baselight not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}