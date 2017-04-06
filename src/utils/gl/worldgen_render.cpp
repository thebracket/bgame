#include "worldgen_render.hpp"
#include "gl_utils.hpp"
#include <rltk.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <iostream>
#include "../utils/gl/gl_utils.hpp"

using namespace rltk;

void worldgen_scene::add_tile(const worldgen_tile &tile) {
    tiles.emplace_back(tile);
}

void worldgen_scene::add_unit(const worldgen_unit &unit) {
    units.emplace_back(unit);
}

void worldgen_scene::render() {
    push_gl_states();

    // Draw it
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 300.0f);//fov, aspect, zNear, zFar

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 112.0f, 10.0f, // Camera
              0.0f, 64.0f, 0.0f, // Target
              0.0f, 1.0f, 0.0f // Up
    );

    glEnable(GL_TEXTURE_2D);
    sf::Texture::bind(rltk::get_texture(term(layer_texture)->get_font_tag()));
    auto texture_size_vec = rltk::get_texture(term(layer_texture)->get_font_tag())->getSize();
    const float texture_w = texture_size_vec.x;
    const float texture_h = texture_size_vec.y;
    glUseProgram(0); // Just in case we left one lying around

    glBegin(GL_QUADS);
    for (const worldgen_tile &tile : tiles) {
        const int tex_x = (tile.glyph % 16) * 24;
        const int tex_y = (tile.glyph / 16) * 24;
        const float tex_xf = (float) tex_x / texture_w;
        const float tex_yf = (float) tex_y / texture_h;
        const float tex_xsize = 24.0f / texture_w;
        const float tex_ysize = 24.0f / texture_h;

        glColor3f(tile.red, tile.green, tile.blue);
        glTexCoord2f(tex_xf, tex_yf);
        glVertex3f(tile.x, tile.alt1, tile.y);

        glColor3f(tile.red, tile.green, tile.blue);
        glTexCoord2f(tex_xf, tex_yf + tex_ysize);
        glVertex3f(tile.x, tile.alt2, tile.y + 1.0f);

        glColor3f(tile.red, tile.green, tile.blue);
        glTexCoord2f(tex_xf + tex_xsize, tex_yf + tex_ysize);
        glVertex3f(tile.x + 1.0f, tile.alt3, tile.y + 1.0f);

        glColor3f(tile.red, tile.green, tile.blue);
        glTexCoord2f(tex_xf + tex_xsize, tex_yf);
        glVertex3f(tile.x + 1.0f, tile.alt4, tile.y);
    }

    for (const worldgen_unit &unit : units) {
        const int tex_x = (unit.glyph % 16) * 24;
        const int tex_y = (unit.glyph / 16) * 24;
        const float tex_xf = (float) tex_x / texture_w;
        const float tex_yf = (float) tex_y / texture_h;
        const float tex_xsize = 24.0f / texture_w;
        const float tex_ysize = 24.0f / texture_h;


        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(tex_xf, tex_yf);
        glVertex3f(unit.x, unit.altitude + 1.0f, unit.y);

        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(tex_xf, tex_yf + tex_ysize);
        glVertex3f(unit.x, unit.altitude + 3.0f, unit.y );

        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(tex_xf + tex_xsize, tex_yf + tex_ysize);
        glVertex3f(unit.x, unit.altitude + 3.0f, unit.y + 2.0f );

        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(tex_xf + tex_xsize, tex_yf);
        glVertex3f(unit.x, unit.altitude + 1.0f, unit.y + 2.0f);
    }
    glEnd();
    pop_gl_states();
}
