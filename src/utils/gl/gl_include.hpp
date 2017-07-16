#pragma once

// This is a convenience wrapper, because we seem to end up declaring OpenGL in a multidude
// of different ways - and that's a bad thing.

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif