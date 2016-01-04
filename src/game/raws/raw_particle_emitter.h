#pragma once

#include <string>
#include "base_raw.h"
#include "../components/particle_emitter_component.h"
#include "../game.h"

using std::string;

namespace raws {

struct raw_particle_emitter : public base_raw {
    raw_particle_emitter() { type = PARTICLE_EMITTER; };
    raw_particle_emitter(const string &msg, const int &time, const int &freq, const chat_emote_color_t col) : message(msg), ttl(time), frequency(freq), color(col) { type=PARTICLE_EMITTER; }
    string message;
    int ttl;    
    int frequency;
    chat_emote_color_t color;
    
    virtual void build_components(entity &parent, const int &x, const int &y, const bool placeholder=false) const {
	if (!placeholder) ECS->add_component(parent, particle_emitter_component(message,ttl,frequency,color));
    }
    
};
  
}
