#include "voxel_model.hpp"
#include <unordered_map>
#include <iostream>
#include <assert.h>
#include "../../bengine/gl_include.hpp"

namespace vox {

    constexpr int voxidx(const int &w, const int &h, const int &d, const int &x, const int &y, const int &z) {
        return (w * h * z) + (w * y) + x;
    }

    bool is_same(const subvoxel &a, const subvoxel &b) {
        return a.r == b.r && a.g == b.g && a.b == b.b;
    }

    void voxel_model::build_model() {
        // Build a cube map
        std::unordered_map<int, subvoxel> cubes;
        for (const auto cube : voxels) {
            const int idx = voxidx(width, depth, height, cube.x, cube.z, cube.y);
            cubes[idx] = cube;
        }
        std::cout << "Starting with " << cubes.size() << " cubes (" << cubes.size() * 36 << " triangles).\n";
        std::vector<float> geometry;

        // Perform greedy voxels on it
        int cube_count = 0;
        while (!cubes.empty()) {
            const auto first_cube = cubes.begin();
            const auto base_idx = first_cube->first;
            const auto voxel_info = first_cube->second;

            int W = 1;
            int H = 1;
            cubes.erase(base_idx);

            int idx_grow_right = base_idx + 1;
            int x_coordinate = voxel_info.x;
            auto right_finder = cubes.find(idx_grow_right);
            while (x_coordinate < width && right_finder != cubes.end() && is_same(voxel_info, right_finder->second)) {
                cubes.erase(idx_grow_right);
                ++W;
                ++idx_grow_right;
                ++x_coordinate;
                right_finder = cubes.find(idx_grow_right);
            }

            /*if (voxel_info.y < height) {
                int y_progress = voxel_info.y + 1;

                while (y_progress < height) {
                    bool possible = true;
                    for (int gx = voxel_info.x; gx < voxel_info.x + width; ++gx) {
                        const int candidate_idx = voxidx(width, height, depth, gx, y_progress, voxel_info.z);
                        auto vfinder = cubes.find(candidate_idx);
                        if (!(vfinder != cubes.end())) possible = false;
                    }
                    if (possible) {
                        ++H;
                        for (int gx = voxel_info.x; gx < voxel_info.x + width; ++gx) {
                            const int candidate_idx = voxidx(width, height, depth, gx, y_progress, voxel_info.z);
                            cubes.erase(candidate_idx);
                        }
                    }

                    ++y_progress;
                }
            }*/

            add_cube_geometry(geometry, voxel_info, W, H, 3);
            ++cube_count;
        }
        std::cout << "Reduced to " << cube_count << " cubes, " << geometry.size() << " triangles.\n";

        // Build VAO/VBO and associate geometry with it
        build_vbo(geometry);

        // voxels.clear();
    }

    void voxel_model::add_cube_geometry(std::vector<float> &v, const subvoxel &voxel,
                                        const float &W, const float &H, const float &texture_id) {
        const float x0 = -0.5f + voxel.x;
        const float x1 = x0 + W;
        const float y0 = -0.5f + voxel.z;
        const float y1 = y0 + 1.0f;
        const float z0 = -0.5f + voxel.y;
        const float z1 = z0 + H;

        //std::cout << "Cube: " << x0 << "-" << y0 << "-" << z0 << " " << W << "x" << H << "\n";

        v.insert(v.end(), {
                x0, y0, z0, 0.0f, 0.0f, -1.0f, voxel.r, voxel.g, voxel.b,
                x1, y0, z0, 0.0f, 0.0f, -1.0f, voxel.r, voxel.g, voxel.b,
                x1, y1, z0, 0.0f, 0.0f, -1.0f, voxel.r, voxel.g, voxel.b,
                x1, y1, z0, 0.0f, 0.0f, -1.0f, voxel.r, voxel.g, voxel.b,
                x0, y1, z0, 0.0f, 0.0f, -1.0f, voxel.r, voxel.g, voxel.b,
                x0, y1, z0, 0.0f, 0.0f, -1.0f, voxel.r, voxel.g, voxel.b,

                x0, y0, z1, 0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,
                x1, y0, z1, 0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,
                x1, y1, z1, 0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,
                x1, y1, z1, 0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,
                x0, y1, z1, 0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,
                x0, y0, z1, 0.0f, 0.0f, 1.0f, voxel.r, voxel.g, voxel.b,

                x0, y1, z1, -1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x0, y1, z0, -1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x0, y0, z0, -1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x0, y0, z0, -1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x0, y0, z1, -1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x0, y1, z1, -1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,

                x1, y1, z1, 1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x1, y1, z0, 1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x1, y0, z0, 1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x1, y0, z0, 1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x1, y0, z1, 1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x1, y1, z1, 1.0f, 0.0f, 0.0f, voxel.r, voxel.g, voxel.b,

                x0, y0, z0, 0.0f, -1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x1, y0, z0, 0.0f, -1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x1, y0, z1, 0.0f, -1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x1, y0, z1, 0.0f, -1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x0, y0, z1, 0.0f, -1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x0, y0, z0, 0.0f, -1.0f, 0.0f, voxel.r, voxel.g, voxel.b,

                x0, y1, z0, 0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x1, y1, z0, 0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x1, y1, z1, 0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x1, y1, z1, 0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x0, y1, z1, 0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b,
                x0, y1, z0, 0.0f, 1.0f, 0.0f, voxel.r, voxel.g, voxel.b
        });
    }

    void voxel_model::build_vbo(std::vector<float> &v) {
        glGenBuffers(1, &vbo_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v.size(), &v[0], GL_STATIC_DRAW);
        n_elements = (int)v.size() / 6;
        std::cout << "Bound as VBO #" << vbo_id << "\n";

        glGenBuffers(1, &instance_vbo_id);
    }

    void voxel_model::render_instances(std::vector<instance_t> &instances) {
        glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizeof(instance_t) * instances.size(), &instances[0], GL_STATIC_DRAW);

        unsigned int tmp_vao = 0;
        glGenVertexArrays(1, &tmp_vao);
        glBindVertexArray(tmp_vao);

        // Bind the consistent elements
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0); // 0 = Vertex Position

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (char *) nullptr + 3 * sizeof(float));
        glEnableVertexAttribArray(1); // 1 = Normals
        glCheckError();

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (char *) nullptr + 6 * sizeof(float));
        glEnableVertexAttribArray(2); // 2 = Color
        glCheckError();

        // Bind the per-element items
        glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_id);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(3); // 0 = Instance Position
        glVertexAttribDivisor(3, 1);

        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (char *) nullptr + 3 * sizeof(float));
        glEnableVertexAttribArray(4); // 1 = Instance Rotation
        glVertexAttribDivisor(4, 1);

        glCheckError();
        glBindVertexArray(0);

        // Perform the render
        //glDrawArraysInstanced(GL_TRIANGLES, 0, (int)instances.size(), n_elements);
        glBindVertexArray(tmp_vao);
        //glDrawArrays(GL_TRIANGLES, 0, n_elements);
        glDrawArraysInstanced(GL_TRIANGLES, 0, n_elements, (int)instances.size());
        glCheckError();

        glBindVertexArray(0);
        glDeleteVertexArrays(1, &tmp_vao);
    }
}