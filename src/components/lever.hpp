#pragma once

#include <rltk.hpp>

using namespace rltk;

struct lever_t {
    lever_t() {}

    std::string xml_identity = "lever_t";
    bool active = false;

    void to_xml(xml_node * c);
    void from_xml(xml_node * c);
};