#include "sunlight.hpp"
#include "../planet/constants.hpp"
#include "../bengine/gl_include.hpp"
#include "../global_assets/shader_storage.hpp"
#include "chunks/constants.hpp"
#include "chunks/chunks.hpp"
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace render {
    namespace sunlight {
        constexpr int SUN_BUFFER_SIZE = 2048;
        std::unique_ptr<depth_fbo_t> sun_fbo;
        std::atomic<bool> sun_changed{true};
        glm::mat4 lightProjection;
        glm::mat4 lightView;
        glm::mat4 lightSpaceMatrix;
        glm::vec3 light_position;
        glm::vec3 light_color;

        void update(const int &screen_w, const int &screen_h) {
            if (!sun_changed) return;

            // If it doesn't exist, create the FBO
            if (!sun_fbo) sun_fbo = std::make_unique<depth_fbo_t>(SUN_BUFFER_SIZE);

            // For now, just use a static light position
            light_position = glm::vec3(128.0f, 384.0f, 10.0f);
            light_color = glm::vec3(1.0f, 1.0f, 1.0f);

            lightProjection = glm::ortho(-192.0f, 192.0f, -192.0f, 192.0f, 32.0f, 512.0f);
            lightView = glm::lookAt(light_position,
                                              glm::vec3((float)REGION_WIDTH/2.0f, (float)REGION_DEPTH/2.0f, (float)REGION_HEIGHT/2.0f),
                                              glm::vec3( 0.0f, 1.0f,  0.0f));
            lightSpaceMatrix = lightProjection * lightView;

            // Render the sun depth-buffer
            glBindFramebuffer(GL_FRAMEBUFFER, sun_fbo->depth_map_fbo);
            glUseProgram(assets::directional_light_shader);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glViewport(0, 0, SUN_BUFFER_SIZE, SUN_BUFFER_SIZE);
            glUniformMatrix4fv(glGetUniformLocation(assets::directional_light_shader, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(lightProjection));
            glUniformMatrix4fv(glGetUniformLocation(assets::directional_light_shader, "view_matrix"), 1, GL_FALSE, glm::value_ptr(lightView));

            glClear(GL_DEPTH_BUFFER_BIT);
            glCheckError();
            for (const auto &chunk : chunks::chunks) {
                const auto idx = chunk.index;
                //std::cout << idx << ", " << chunks::chunks[idx].has_geometry << "\n";
                if (chunks::chunks[idx].has_geometry && chunks::chunks[idx].ready) {
                    // Render backwards to maximize z-buffer efficiency
                    for (int z=chunks::CHUNK_SIZE-1; z>=0; --z) {
                        const int layer_z = z + chunks::chunks[idx].base_z;
                        if (chunks::chunks[idx].layers[z].vao > 0) {
                            //std::cout << "Activating array " << idx << ":" << layer_z << ", " << chunks::chunks[idx].layers[z].n_elements << "\n";
                            glBindVertexArray(chunks::chunks[idx].layers[z].vao);
                            glDrawArrays(GL_TRIANGLES, 0, chunks::chunks[idx].layers[z].n_elements);
                        }
                    }
                }
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, screen_w, screen_h);
            glDisable(GL_DEPTH_TEST);
            glCheckError();

            sun_changed = false;
        }

        

    }
}