#pragma once

#include <rltk.hpp>
#include "../raws/items.hpp"
#include <bitset>

using namespace rltk;

struct stockpile_t {

    stockpile_t() {}
    std::string xml_identity = "stockpile_t";

    std::bitset<NUMBER_OF_ITEM_CATEGORIES> category;

    void to_xml(xml_node * c);
    void from_xml(xml_node * c);
};