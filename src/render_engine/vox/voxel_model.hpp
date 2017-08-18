#pragma once

#include <vector>
#include <unordered_map>

namespace vox {

    struct subvoxel {
        int x, y, z;
        float r, g, b;
    };

    struct instance_t {
        float x,y,z;
        float axis1, axis2, axis3, rot_angle;
    };

    struct voxel_model {
        int width, height, depth;
        std::vector<subvoxel> voxels;
        unsigned int vbo_id = 0;
        unsigned int instance_vbo_id = 0;
        int n_elements = 0;

        void build_model();
        void render_instances(std::vector<instance_t> &instances);

    private:
        void add_cube_geometry(std::vector<float> &v, const subvoxel &voxel,
                               const float &width, const float &height, const float &texture_id);

        void build_vbo(std::vector<float> &v);
    };
}