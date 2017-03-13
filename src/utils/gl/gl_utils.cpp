#include "gl_utils.hpp"
#include <rltk.hpp>

using namespace rltk;

void push_gl_states() {
    get_window()->pushGLStates();
    get_window()->resetGLStates();
}

void pop_gl_states() {
    get_window()->popGLStates();
}

