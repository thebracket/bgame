#include "sun_fbo.hpp"
#include <rltk.hpp>

namespace map_render {

    bool loaded_sun_fbo = false;

    // Shadow-based lighting
    GLuint sun_fbo;
    GLuint sun_depth_buffer;
    GLuint sun_depth_texture;
    GLuint sun_render;

    void load_sun_fbo() {
        const auto screen_size = rltk::get_window()->getSize();
        glGenFramebuffers(1, &sun_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, sun_fbo);
        std::cout << "Sun FBO: " << sun_fbo << "\n";

        // Depth buffer
        glGenRenderbuffers(1, &sun_depth_buffer);
        std::cout << "Sun depth buffer: " << sun_depth_buffer << "\n";
        glBindRenderbuffer(GL_RENDERBUFFER, sun_depth_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_size.x, screen_size.y);
        glBindFramebuffer(GL_FRAMEBUFFER, sun_fbo);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, sun_depth_buffer);

        // Depth texture
        glGenTextures(1, &sun_depth_texture);
        glBindTexture(GL_TEXTURE_2D, sun_depth_texture);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screen_size.x, screen_size.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
        std::cout << "Sun depth texture: " << sun_depth_texture << "\n";
        glBindFramebuffer(GL_FRAMEBUFFER, sun_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sun_depth_texture, 0);

        // Render texture
        glGenTextures(1, &sun_render);
        glBindTexture(GL_TEXTURE_2D, sun_render);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_size.x, screen_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, sun_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sun_render, 0);

        GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT  };
        glDrawBuffers(1, buffers);

        // Return to regular render mode
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        loaded_sun_fbo = true;
        std::cout << "Setup Shadow/Sun FBO\n";
    }
}