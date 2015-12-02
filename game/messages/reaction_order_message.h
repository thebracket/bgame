#pragma once

#include <string>
#include <utility>
#include <vector>

using std::string;

struct reaction_order_message {
    reaction_order_message() {  }
    reaction_order_message( const string &name, const int &workshop, const std::vector<std::pair<string,int>> &req, const std::vector<std::pair<string,int>> &makes, const string &say, const int &power ) 
	  : reaction_name(name), workshop_id(workshop), requirements(req), creates(makes), emote(say), power_drain(power) {}
  
    bool deleted = false;
    int ttl = 2;

    string reaction_name;
    int workshop_id;
    std::vector<std::pair<string,int>> requirements;
    std::vector<std::pair<string,int>> creates;
    string emote;
    int power_drain;
};