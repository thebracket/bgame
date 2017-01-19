#pragma once

#include <rltk.hpp>

using namespace rltk;

enum trigger_type { trigger_cage, trigger_stonefall, trigger_blade };

struct entry_trigger_t {
    bool active = true;
    trigger_type type;

    entry_trigger_t() {}
    entry_trigger_t(const trigger_type &t) : type(t) {}

    std::string xml_identity = "entry_trigger_t";

    void to_xml(xml_node * c);
    void from_xml(xml_node * c);
};