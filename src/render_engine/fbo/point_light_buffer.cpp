#include "point_light_buffer.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../global_assets/game_config.hpp"

namespace render {
	point_light_buffer_t::point_light_buffer_t(const int &w, const int &h) {
		width = config::game_config.shadow_map_size;
		height = width;

		glGenFramebuffers(1, &fbo_id);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);

		glGenTextures(1, &depth_cubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depth_cubemap);
		for (unsigned int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT16, w, w, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_cubemap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			glog(log_target::LOADER, log_severity::error, "Framebuffer pointlight not complete!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	point_light_buffer_t::~point_light_buffer_t() {
		if (fbo_id > 0) {
			glDeleteFramebuffers(1, &fbo_id);
		}
		if (depth_cubemap > 0) {
			glDeleteTextures(1, &depth_cubemap);
		}
	}
}
