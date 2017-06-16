#include "main_fbo.hpp"
#include <rltk.hpp>

namespace map_render {
    bool loaded_fbo = false;

    GLuint mouse_pick_texture;
    GLuint mouse_pick_fbo;
    GLuint mouse_pick_depth;
    GLuint render_texture;
    GLuint normal_texture;
    GLuint interpolated_pos_texture;

    void load_fbo() {
        // Create and bind the framebuffer for mouse-picking output
        glGenFramebuffers(1, &mouse_pick_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);

        // Create the mouse picker render target texture
        const auto screen_size = rltk::get_window()->getSize();
        glGenTextures(1, &mouse_pick_texture);
        glBindTexture(GL_TEXTURE_2D, mouse_pick_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_size.x, screen_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mouse_pick_texture, 0);

        // Create the render target texture
        glGenTextures(1, &render_texture);
        glBindTexture(GL_TEXTURE_2D, render_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_size.x, screen_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, render_texture, 0);

        // Create the normal target texture
        glGenTextures(1, &normal_texture);
        glBindTexture(GL_TEXTURE_2D, normal_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_size.x, screen_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, normal_texture, 0);

        // Create the interpolated position target texture
        glGenTextures(1, &interpolated_pos_texture);
        glBindTexture(GL_TEXTURE_2D, interpolated_pos_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_size.x, screen_size.y, 0, GL_RGB, GL_UNSIGNED_INT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, interpolated_pos_texture, 0);

        // Create a depth-buffer for the render target
        glGenRenderbuffers(1, &mouse_pick_depth);
        glBindRenderbuffer(GL_RENDERBUFFER, mouse_pick_depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_size.x, screen_size.y);
        glBindFramebuffer(GL_FRAMEBUFFER, mouse_pick_fbo);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mouse_pick_depth);

        GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT };
        glDrawBuffers(4, buffers);

        // Return to regular render mode
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        loaded_fbo = true;
    }
}
