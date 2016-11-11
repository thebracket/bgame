#include "sentient_ai.hpp"

void sentient_ai::to_xml(xml_node * c) {
    component_to_xml(c,
        std::make_pair("initiative", initiative),
        std::make_pair("initiative_modifier", initiative_modifier),
        std::make_pair("person_id", person_id),
        std::make_pair("aggression", aggression),
        std::make_pair("hostile", hostile),
        std::make_pair("goal", goal),
        std::make_pair("target", target),
        std::make_pair("days_since_arrival", days_since_arrival)
    );
}

void sentient_ai::from_xml(xml_node * c) {
    initiative = c->val<int>("initiative");
    initiative_modifier = c->val<int>("initiative_modifier");
    person_id = c->val<std::size_t>("person_id");
    aggression = c->val<int>("aggression");
    hostile = c->val<bool>("hostile");
    goal = (sentient_goal_t)c->val<int>("goal");
    target = c->val<std::size_t>("target");
    days_since_arrival = c->val<int>("days_since_arrival");
}
