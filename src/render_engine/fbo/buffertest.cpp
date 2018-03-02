#include "buffertest.hpp"
#include "../../global_assets/shader_storage.hpp"
#include "../../bengine/gl_include.hpp"

namespace render {

    namespace testimpl {
        unsigned int quadVAO = 0;
        unsigned int quadVBO = 0;
    }

    inline void check_quad() {
        using namespace testimpl;
        if (quadVAO == 0) {
            float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                    // positions   // texCoords
                    -1.0f,  1.0f,  0.0f, 1.0f,
                    -1.0f, -1.0f,  0.0f, 0.0f,
                    1.0f, -1.0f,  1.0f, 0.0f,

                    -1.0f,  1.0f,  0.0f, 1.0f,
                    1.0f, -1.0f,  1.0f, 0.0f,
                    1.0f,  1.0f,  1.0f, 1.0f
            };

            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glCheckError();
        }
    }

    void render_fullscreen_quad(unsigned int &texture) {
        using namespace testimpl;
        check_quad();

        // Render the depth buffer to the screen
        glUseProgram(assets::depthquad_shader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glCheckError();
    }

	void render_fullscreen_quad_untextured() {
		using namespace testimpl;
		check_quad();

		// Render the depth buffer to the screen
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

    void render_buffer_quad() {
        using namespace testimpl;
        check_quad();

        // Render the depth buffer to the screen
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glCheckError();
    }
}