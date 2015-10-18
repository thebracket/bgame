#pragma once

#include "base_component.h"
#include "../virtual_terminal.h"

namespace engine {
namespace ecs {

class renderable_component : public base_component {
public:
    renderable_component() {
        type = renderable;
    };
    renderable_component(const unsigned char &GLYPH, const engine::vterm::color_t &FOREGROUND, const engine::vterm::color_t &BACKGROUND) :
        glyph(GLYPH), foreground(FOREGROUND), background(BACKGROUND)
    {
        type = renderable;
    };
    unsigned char glyph;
    engine::vterm::color_t foreground;
    engine::vterm::color_t background;
};

}
}
