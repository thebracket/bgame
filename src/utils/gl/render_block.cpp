#include "render_block.hpp"
#include "../../main/game_globals.hpp"

/* Texture information - stores the size of the texture we are using */
float texture_w = 384.0F;
float texture_h = 768.0F;
float tex_xsize = 24.0f / texture_w;
float tex_ysize = 24.0f / texture_h;

void calculate_texture_info() {
    auto texture_size_vec = rltk::get_texture(term(1)->get_font_tag())->getSize();
    texture_w = texture_size_vec.x;
    texture_h = texture_size_vec.y;
    tex_xsize = (24.0f / texture_w);
    tex_ysize = (24.0f / texture_h);
}

void render_block::reset() {
    vertices.clear();
    tvertices.clear();
    cvertices.clear();
}

float render_block::light_red(const rltk::vchar &c) const noexcept {
    return (float)c.foreground.r / 255.0f;
}

float render_block::light_green(const rltk::vchar &c) const noexcept {
    return (float)c.foreground.g / 255.0f;
}

float render_block::light_blue(const rltk::vchar &c) const noexcept {
    return (float)c.foreground.b / 255.0f;
}

void render_block::add_floor(const int &x, const int &y, const int &z, const rltk::vchar &c)
{
    const float light_r = light_red(c) - ((camera_position->region_z - (float)z)*0.05f);
    const float light_g = light_green(c) - ((camera_position->region_z - (float)z)*0.05f);
    const float light_b = light_blue(c) - ((camera_position->region_z - (float)z)*0.05f);

    const float X = (float)x;
    const float Y = (float)y;
    const float Z = (float)z;
    const float vsize = 1.0f;

    const float tex_x = (c.glyph % 16) * 24.0f;
    const float tex_y = (c.glyph / 16) * 24.0f;
    const float tex_xf = (float) tex_x / texture_w;
    const float tex_yf = (float) tex_y / texture_h;

    // Floor
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});

    tvertices.emplace_back(t3d{tex_xf, tex_yf});
    tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

    vertices.emplace_back(v3d{X,        Z, Y});
    vertices.emplace_back(v3d{X,        Z, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z, Y});

    const float SX = X/255.0f;
    const float SY = Y/255.0f;
    const float SZ = Z/255.0f;
    screen_index.emplace_back(v3d{SX,SY,SZ});
    screen_index.emplace_back(v3d{SX,SY,SZ});
    screen_index.emplace_back(v3d{SX,SY,SZ});
    screen_index.emplace_back(v3d{SX,SY,SZ});

    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
}

void render_block::add_decal(const int &x, const int &y, const int &z, const rltk::vchar &c)
{
    const float light_r = 1.0f - ((camera_position->region_z - (float)z)*0.05f);
    const float light_g = 1.0f - ((camera_position->region_z - (float)z)*0.05f);
    const float light_b = 1.0f - ((camera_position->region_z - (float)z)*0.05f);

    const float X = (float)x;
    const float Y = (float)y;
    const float Z = (float)z;
    const float vsize = 1.0f;

    const float tex_x = ((c.glyph % 16) * 24.0f);
    const float tex_y = ((c.glyph / 16) * 24.0f);
    const float tex_xf = (tex_x / texture_w);
    const float tex_yf = (tex_y / texture_h);

    // Floor
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});

    tvertices.emplace_back(t3d{tex_xf, tex_yf});
    tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

    vertices.emplace_back(v3d{X,        Z, Y});
    vertices.emplace_back(v3d{X,        Z, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z, Y});

    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
}

void render_block::add_cube(const int &x, const int &y, const int &z, const rltk::vchar &c) {
    const float light_r = light_red(c);
    const float light_g = light_green(c);
    const float light_b = light_blue(c);

    const float X = (float)x;
    const float Y = (float)y;
    const float Z = (float)z;
    const float vsize = 1.0f;

    const float tex_x = (c.glyph % 16) * 24.0f;
    const float tex_y = (c.glyph / 16) * 24.0f;
    const float tex_xf = (float) tex_x / texture_w;
    const float tex_yf = (float) tex_y / texture_h;

    // Floor
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});

    tvertices.emplace_back(t3d{tex_xf, tex_yf});
    tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

    vertices.emplace_back(v3d{X,        Z, Y});
    vertices.emplace_back(v3d{X,        Z, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z, Y});

    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});

    // Left Face
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});

    tvertices.emplace_back(t3d{tex_xf, tex_yf});
    tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

    vertices.emplace_back(v3d{X, Z, Y});
    vertices.emplace_back(v3d{X, Z, Y + vsize});
    vertices.emplace_back(v3d{X, Z + vsize, Y + vsize});
    vertices.emplace_back(v3d{X, Z + vsize, Y});

    normals.emplace_back(v3d{-1.0f, 0.0f, 0.0f});
    normals.emplace_back(v3d{-1.0f, 0.0f, 0.0f});
    normals.emplace_back(v3d{-1.0f, 0.0f, 0.0f});
    normals.emplace_back(v3d{-1.0f, 0.0f, 0.0f});

    // Right Face
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});

    tvertices.emplace_back(t3d{tex_xf, tex_yf});
    tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

    vertices.emplace_back(v3d{X + vsize, Z, Y});
    vertices.emplace_back(v3d{X + vsize, Z, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z + vsize, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z + vsize, Y});

    normals.emplace_back(v3d{1.0f, 0.0f, 0.0f});
    normals.emplace_back(v3d{1.0f, 0.0f, 0.0f});
    normals.emplace_back(v3d{1.0f, 0.0f, 0.0f});
    normals.emplace_back(v3d{1.0f, 0.0f, 0.0f});

    // North face
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});

    tvertices.emplace_back(t3d{tex_xf, tex_yf});
    tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

    vertices.emplace_back(v3d{X, Z, Y});
    vertices.emplace_back(v3d{X + vsize, Z, Y});
    vertices.emplace_back(v3d{X + vsize, Z + vsize, Y});
    vertices.emplace_back(v3d{X, Z + vsize, Y});

    normals.emplace_back(v3d{0.0f, 0.0f, -1.0f});
    normals.emplace_back(v3d{0.0f, 0.0f, -1.0f});
    normals.emplace_back(v3d{0.0f, 0.0f, -1.0f});
    normals.emplace_back(v3d{0.0f, 0.0f, -1.0f});

    // South face
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});

    tvertices.emplace_back(t3d{tex_xf, tex_yf});
    tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

    vertices.emplace_back(v3d{X, Z, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z + vsize, Y + vsize});
    vertices.emplace_back(v3d{X, Z + vsize, Y + vsize});

    normals.emplace_back(v3d{0.0f, 0.0f, 1.0f});
    normals.emplace_back(v3d{0.0f, 0.0f, 1.0f});
    normals.emplace_back(v3d{0.0f, 0.0f, 1.0f});
    normals.emplace_back(v3d{0.0f, 0.0f, 1.0f});

    // Ceiling - facing up because that's what is visible
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});

    tvertices.emplace_back(t3d{tex_xf, tex_yf});
    tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

    vertices.emplace_back(v3d{X,        Z+vsize, Y});
    vertices.emplace_back(v3d{X,        Z+vsize, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z+vsize, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z+vsize, Y});

    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
}

void render_block::render() const noexcept {
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, &cvertices[0]);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, &tvertices[0]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, &normals[0]);

    glDrawArrays(GL_QUADS, 0, vertices.size());

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void render_block::render_index() const noexcept {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, &screen_index[0]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);

    glDrawArrays(GL_QUADS, 0, vertices.size());

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
