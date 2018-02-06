#pragma once

#include <vector>
#include <string>

/*
 * Defines a template for events that can happen to a settler during character generation.
 */
struct life_event_template {
    int min_age = 0;
    int max_age = 0;
    std::string description = "";
    int weight = 0;
    short strength = 0;
    short dexterity = 0;
    short constitution = 0;
    short intelligence = 0;
    short wisdom = 0;
    short charisma = 0;
    short comeliness = 0;
    short ethics = 0;
    std::vector<std::string> skills;
    std::vector<std::string> requires_event;
    std::vector<std::string> precludes_event;
};
