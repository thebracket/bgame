#include "hdr_buffer.hpp"
#include "../../bengine/gl_include.hpp"

namespace render {
    hdr_buffer_t::hdr_buffer_t(const int &w, const int &h) {
        width = w;
        height = h;

        glGenFramebuffers(1, &fbo_id);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
        // position color buffer
        glGenTextures(1, &color_tex);
        glBindTexture(GL_TEXTURE_2D, color_tex);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_tex, 0);

        // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
        unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, attachments);

        // create and attach depth buffer (renderbuffer)
        glGenRenderbuffers(1, &rbo_id);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_id);

        // finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			glog(log_target::LOADER, log_severity::error, "Framebuffer hdr not complete!");
		}
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}