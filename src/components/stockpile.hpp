#pragma once

#include <rltk.hpp>
#include <bitset>

using namespace rltk;

struct stockpile_t {

    stockpile_t() {}
    std::string xml_identity = "stockpile_t";

    std::bitset<128> category;

    void to_xml(xml_node * c);
    void from_xml(xml_node * c);
};