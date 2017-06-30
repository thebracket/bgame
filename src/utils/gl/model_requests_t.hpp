#pragma once

namespace gl {

    struct static_model_t {
        int model_id;
        float x,y,z;
        bool above_ground;
        float light_x, light_y, light_z, light_r, light_g, light_b;
    };

    struct model_request_t {
        boost::container::flat_map<int, std::vector<static_model_t>> models;

        void add_model_request(const int &model_id, const static_model_t &model) {
            models[model_id].emplace_back(model);
        }
    };

}