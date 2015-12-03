#pragma once

#include <string>

using std::string;

struct inflict_damage_message {
    inflict_damage_message() {  }
    inflict_damage_message(const int &entity, const int &damage, const string &type, const int &tx, const int &ty) 
      : target_entity(entity), damage_taken(damage), damage_source(type), tile_x(tx), tile_y(ty) {}
  
    bool deleted = false;
    int ttl = 2;
    
    int target_entity;
    int damage_taken;
    string damage_source;
    int tile_x, tile_y;
};
