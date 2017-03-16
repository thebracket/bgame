#pragma once

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

void push_gl_states();
void pop_gl_states();
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);