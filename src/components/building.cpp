#include "building.hpp"

void building_t::to_xml(xml_node * c) {
    component_to_xml(c, 
        std::make_pair("tag", tag),
        std::make_pair("width", width),
        std::make_pair("height", height),
        std::make_pair("complete", complete),
        std::make_pair("glyphs", glyphs),
        std::make_pair("built_with", built_with),
        std::make_pair("civ_owner", civ_owner)
    );    
};

void building_t::from_xml(xml_node * c) {
    tag = c->val<std::string>("tag");
    width = c->val<int>("width");
    height = c->val<int>("height");
    c->iterate_child("glyphs", [this] (xml_node * ch) {
        glyphs.push_back(ch->vchar());
    });
    complete = c->val<bool>("complete");
    c->iterate_child("built_with", [this] (xml_node * ch) {
        glyphs.push_back(ch->vchar());        
    });
    civ_owner = c->val<std::size_t>("civ_owner");
}
