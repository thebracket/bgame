#pragma once

#include <string>

using std::string;

struct inflict_damage_message {
    inflict_damage_message() {  }
    inflict_damage_message(const int &entity, const int &damage, const string &type) : target_entity(entity), damage_taken(damage), damage_source(type) {}
  
    bool deleted = false;
    int ttl = 2;
    
    int target_entity;
    int damage_taken;
    string damage_source;
};
