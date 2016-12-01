#include "building_designation_t.hpp"

void building_designation_t::to_xml(rltk::xml_node * c) {
    component_to_xml(c,
        std::make_pair("x", x),
        std::make_pair("y", y),
        std::make_pair("z", z),
        std::make_pair("component_ids", component_ids),
        std::make_pair("name", name),
        std::make_pair("tag", tag),
        std::make_pair("components", components),
        std::make_pair("width", width),
        std::make_pair("height", height),
        std::make_pair("glyphs", glyphs),
        std::make_pair("building_entity", building_entity)
    );
}

void building_designation_t::from_xml(rltk::xml_node * c) {
    x = c->val<int>("x");
    y = c->val<int>("y");
    z = c->val<int>("z");
    name = c->val<std::string>("name");
    tag = c->val<std::string>("tag");
    width = c->val<int>("width");
    height = c->val<int>("height");
    c->iterate_child("component_ids", [this] (rltk::xml_node * child) {
        std::pair<std::size_t, bool> component{ child->find("component_ids_first")->val<std::size_t>("component_ids"),
            child->find("component_ids_second")->val<std::size_t>("component_ids") };
        component_ids.push_back(component);
    });
    c->iterate_child("components", [this] (rltk::xml_node * child) {
        reaction_input_t r;
        r.from_xml(child);
        components.push_back(r);
    });
    c->iterate_child("glyphs", [this] (rltk::xml_node * child) {
        rltk::vchar g = child->vchar();
        glyphs.push_back(g);
    });

    building_entity = c->val<std::size_t>("building_entity");
}

