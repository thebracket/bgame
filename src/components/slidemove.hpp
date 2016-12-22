#pragma once

#include <rltk.hpp>

using namespace rltk;

struct slidemove_t {
    float offsetX = 0;
    float offsetY = 0;
    float offsetZ = 0;
    int lifespan = 0;

    slidemove_t() {}
    slidemove_t(const float &x, const float &y, const float &z, const int &life) :
        offsetX(x), offsetY(y), offsetZ(z), lifespan(life) {}
    std::string xml_identity = "slidemove_t";

    void to_xml(xml_node * c);
    void from_xml(xml_node * c);
};