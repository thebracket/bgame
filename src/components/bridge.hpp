#pragma once

#include <rltk.hpp>

using namespace rltk;

struct bridge_t {
    bool complete = false;

    bridge_t() {}

    std::string xml_identity = "bridge_t";

    void to_xml(xml_node * c);
    void from_xml(xml_node * c);
};