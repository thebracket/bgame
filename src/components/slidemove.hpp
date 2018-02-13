#pragma once

struct slidemove_t {
    float offsetX = 0;
    float offsetY = 0;
    float offsetZ = 0;
    int lifespan = 0;

	slidemove_t() = default;
    slidemove_t(const float &x, const float &y, const float &z, const int &life) noexcept :
        offsetX(x), offsetY(y), offsetZ(z), lifespan(life) {}

};

