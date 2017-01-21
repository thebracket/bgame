#pragma once

#include <rltk.hpp>

using namespace rltk;

struct receives_signal_t {
    receives_signal_t() {}

    std::string xml_identity = "receives_signal_t";
    bool active = false;

    void to_xml(xml_node * c);
    void from_xml(xml_node * c);
};