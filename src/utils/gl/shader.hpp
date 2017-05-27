#pragma once

#include <string>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif

struct shader_t {
    shader_t(const char * gfn, const char * vfn, const char * ffn) : geometry_filename(gfn),
        vertex_filename(vfn), fragment_filename(ffn)
    {
        load_shader(geometry_filename, vertex_filename, fragment_filename);
    }

    void activate();
    void deactivate();

private:
    const char * geometry_filename;
    const char * vertex_filename;
    const char * fragment_filename;
    void load_shader(const char * geometry_shader_path, const char * vertex_file_path,const char * fragment_file_path);
    std::string read_code_file(const char * filename);
    void compile_shader(GLuint &id, std::string &code, const char * filename);
    GLuint shader_id = 0;
};