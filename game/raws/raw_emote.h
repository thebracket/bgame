#pragma once

#include <string>
#include "base_raw.h"
#include "../game.h"

using std::string;

namespace raws {

struct raw_emote : public base_raw {
    raw_emote() { type = EMOTE; };
    raw_emote(const string &new_emote) : emote ( new_emote ) { type=EMOTE; }
    string emote;    
};
  
}
