#include "directional_light_buffer.hpp"
#include "../../bengine/gl_include.hpp"

namespace render {
	directional_light_buffer_t::directional_light_buffer_t(const int &w, const int &h) {
		width = w;
		height = h;

		glGenFramebuffers(1, &fbo_id);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);

		// Depth texture (float for more precision)
		glGenTextures(1, &depth_tex);
		glBindTexture(GL_TEXTURE_2D, depth_tex);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, width, height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex, 0);

		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}