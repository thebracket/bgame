#include "initiative.hpp"

using namespace rltk;

void initiative_t::to_xml(xml_node * c) {
    component_to_xml(c,
                     std::make_pair("initiative", initiative),
                     std::make_pair("initiative_modifier", initiative_modifier)
    );
}

void initiative_t::from_xml(xml_node * c) {
    initiative = c->val<int>("initiative");
    initiative_modifier = c->val<int>("initiative_modifier");
}
