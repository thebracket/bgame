#include "reaction_task_t.hpp"

void reaction_task_t::to_xml(rltk::xml_node * c) {
    rltk::component_to_xml(c,
        std::make_pair("building_id", building_id),
        std::make_pair("job_name", job_name),
        std::make_pair("reaction_tag", reaction_tag),
        std::make_pair("components", components)
    );
}

void reaction_task_t::from_xml(rltk::xml_node * c) {
    building_id = c->val<std::size_t>("building_id");
    job_name = c->val<std::string>("job_name");
    reaction_tag = c->val<std::string>("reaction_tag");
    c->iterate_child("components", [this] (rltk::xml_node *ch) {
        std::pair<std::size_t,bool> comp{ 
            ch->find("components_first")->val<std::size_t>("components"),
            ch->find("components_second")->val<std::size_t>("components")
        };
        components.push_back(comp);
    });
}
