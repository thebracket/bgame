#include "gbuffer.hpp"
#include "../../bengine/gl_include.hpp"

namespace render {
    gbuffer_t::gbuffer_t(const int &w, const int &h) {
        width = w;
        height = h;

        glGenFramebuffers(1, &fbo_id);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);

        // position color buffer
        glGenTextures(1, &position_tex);
        glBindTexture(GL_TEXTURE_2D, position_tex);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, width, height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position_tex, 0);

        // normal color buffer
        glGenTextures(1, &normal_tex);
        glBindTexture(GL_TEXTURE_2D, normal_tex);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, width, height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal_tex, 0);

        // color buffer
        glGenTextures(1, &albedo_tex);
        glBindTexture(GL_TEXTURE_2D, albedo_tex);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedo_tex, 0);

        // ambient occlusion texture
        glGenTextures(1, &ao_tex);
        glBindTexture(GL_TEXTURE_2D, ao_tex);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, ao_tex, 0);

		// Depth texture (float for more precision)
		glGenTextures(1, &depth_tex);
		glBindTexture(GL_TEXTURE_2D, depth_tex);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, width, height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex, 0);

        // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
        unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
                                        GL_COLOR_ATTACHMENT3};
        glDrawBuffers(4, attachments);

        // create and attach depth buffer (renderbuffer)
        //glGenRenderbuffers(1, &rbo_id);
        //glBindRenderbuffer(GL_RENDERBUFFER, rbo_id);
        //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_id);

        // finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			glog(log_target::LOADER, log_severity::warning, "(GBuffer) Framebuffer not complete!");
		}
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}