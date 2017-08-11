#include "sunlight.hpp"
#include "../planet/constants.hpp"
#include "../bengine/gl_include.hpp"
#include "../global_assets/shader_storage.hpp"
#include "chunks/constants.hpp"
#include "chunks/chunks.hpp"
#include "../global_assets/game_camera.hpp"
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace render {
    namespace sunlight {
        constexpr int SUN_BUFFER_SIZE = 128;
        std::unique_ptr<depth_fbo_t> sun_fbo;
        std::atomic<bool> sun_changed{true};
        glm::mat4 lightProjection;
        glm::mat4 lightView;
        glm::mat4 lightSpaceMatrix;
        glm::vec3 light_position;
        glm::vec3 light_color;
        double time_since_last_update = 1000.0;

        void update(const int &screen_w, const int &screen_h, const double &duration_ms) {
            time_since_last_update += duration_ms;
            if (!sun_changed) return;

            // For now, just use a static light position
            //light_position = glm::vec3(128.0f, 256.0f, 130.0f);
            const glm::vec3 target{(float) REGION_WIDTH/2.0f, (float)REGION_DEPTH/2.0f, (float)REGION_HEIGHT/2.0f};
            light_color = glm::vec3(1.0f, 1.0f, 1.0f);
            const glm::vec3 up{0.0f, 1.0f, 0.0f};

            constexpr float SUN_DISTANCE = 512.0f;
            const float radians = time_since_last_update * 3.14159265f/180.0f;
            float sun_x = (SUN_DISTANCE * std::cos(radians)) + 128.0f;
            float sun_y = (SUN_DISTANCE * std::sin(radians)) + 128.0f;
            float sun_z = 129.0f;
            light_position = glm::vec3(sun_x, sun_y, sun_z);

            lightProjection = glm::ortho(0.0f, 256.0f, 0.0f, 256.0f, 32.0f, 512.0f);
            //lightProjection = glm::perspective(90.0f, 1.0f, 1.0f, 512.0f);
            lightView = glm::lookAt(light_position, target, up);
            lightSpaceMatrix = lightProjection * lightView;

            // Render the sun depth-buffer
            glUseProgram(assets::directional_light_shader);
            glBindFramebuffer(GL_FRAMEBUFFER, sun_fbo->depth_map_fbo);
            glClear(GL_DEPTH_BUFFER_BIT);

            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glViewport(0, 0, SUN_BUFFER_SIZE, SUN_BUFFER_SIZE);
            glUniformMatrix4fv(glGetUniformLocation(assets::directional_light_shader, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(lightProjection));
            glUniformMatrix4fv(glGetUniformLocation(assets::directional_light_shader, "view_matrix"), 1, GL_FALSE, glm::value_ptr(lightView));

            for (const auto &chunk : chunks::chunks) {
                const auto idx = chunk.index;
                //std::cout << idx << ", " << chunks::chunks[idx].has_geometry << "\n";
                if (chunks::chunks[idx].has_geometry && chunks::chunks[idx].ready) {
                    // Render backwards to maximize z-buffer efficiency
                    for (int z=chunks::CHUNK_SIZE-1; z>=0; --z) {
                        const int layer_z = z + chunks::chunks[idx].base_z;
                        if (chunks::chunks[idx].layers[z].vao > 0 && chunks::chunks[idx].layers[z].n_elements > 0) {
                            //std::cout << "Activating array " << idx << ":" << layer_z << ", " << chunks::chunks[idx].layers[z].n_elements << "\n";
                            glBindVertexArray(chunks::chunks[idx].layers[z].vao);
                            glDrawArrays(GL_TRIANGLES, 0, chunks::chunks[idx].layers[z].n_elements);
                        }
                    }
                }
            }

            glViewport(0, 0, screen_w, screen_h);

            //sun_changed = false;
        }

        

    }
}