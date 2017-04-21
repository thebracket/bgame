#include "render_block.hpp"
#include "../../main/game_camera.hpp"

/* Texture information - stores the size of the texture we are using */
float texture_w = 384.0F;
float texture_h = 768.0F;
float texture_glyph_size = 24.0F;
float tex_xsize = 24.0f / texture_w;
float tex_ysize = 24.0f / texture_h;

void calculate_texture_info() {
    auto texture_size_vec = rltk::get_texture(term(1)->get_font_tag())->getSize();
    texture_glyph_size = texture_size_vec.x / 16;
    texture_w = texture_size_vec.x;
    texture_h = texture_size_vec.y;
    tex_xsize = (24.0f / texture_w);
    tex_ysize = (24.0f / texture_h);
}

void render_block::reset() {
    vertices.clear();
    tvertices.clear();
    cvertices.clear();
    screen_index.clear();
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

std::tuple<float,float,float> render_block::altitude_color_bias(const rltk::vchar &c, const int &z) const noexcept {
    return std::make_tuple(
            light_red(c) - ((camera_position->region_z - (float)z)*0.05f),
            light_green(c) - ((camera_position->region_z - (float)z)*0.05f),
            light_blue(c) - ((camera_position->region_z - (float)z)*0.05f)
    );
}

std::tuple<float, float, float, float, float, float, float, float> render_block::size_calcs(const rltk::vchar &c) const noexcept
{
    const float tex_x = (c.glyph % 16) * texture_glyph_size;
    const float tex_y = (c.glyph / 16) * texture_glyph_size;

    return std::make_tuple(
            -0.5f,
            -0.5f,
            -0.5f,
            1.0f,
            tex_x,
            tex_y,
            tex_x / texture_w,
            tex_y / texture_h
    );
}

std::tuple<float, float, float> render_block::screenspace_calc(const int &x, const int &y, const int &z) const noexcept {
    return std::make_tuple((float)x/255.0f, (float)y/255.0f, (float)z/255.0f);
};

void render_block::primitives_floor(const float &light_r, const float &light_g, const float &light_b, const float &tex_xf,
                                    const float &tex_yf, const float &SX, const float &SY, const float &SZ, const float &billboard_mode)
{
    // Colors
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});

    // Texture locs
    tvertices.emplace_back(t3d{tex_xf, tex_yf});
    tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

    // Vertices (relative to 0.0)
    vertices.emplace_back( v3d{ -0.5f, -0.5f, -0.5f } );
    vertices.emplace_back( v3d{ -0.5f, -0.5f,  0.5f } );
    vertices.emplace_back( v3d{  0.5f, -0.5f,  0.5f } );
    vertices.emplace_back( v3d{  0.5f, -0.5f, -0.5f } );

    // Screen index
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});

    // Normals
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
}

void render_block::primitives_cube_left(const float &light_r, const float &light_g, const float &light_b, const float &tex_xf,
                                    const float &tex_yf, const float &SX, const float &SY, const float &SZ, const float &billboard_mode,
                                    const float &X, const float &Y, const float &Z, const float &vsize, const float height)
{
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});

    tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});
    tvertices.emplace_back(t3d{tex_xf, tex_yf});

    vertices.emplace_back(v3d{X, Z, Y});
    vertices.emplace_back(v3d{X, Z, Y + vsize});
    vertices.emplace_back(v3d{X, Z + height, Y + vsize});
    vertices.emplace_back(v3d{X, Z + height, Y});

    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});

    normals.emplace_back(v3d{-1.0f, 0.0f, 0.0f});
    normals.emplace_back(v3d{-1.0f, 0.0f, 0.0f});
    normals.emplace_back(v3d{-1.0f, 0.0f, 0.0f});
    normals.emplace_back(v3d{-1.0f, 0.0f, 0.0f});
}

void render_block::primitives_cube_right(const float &light_r, const float &light_g, const float &light_b, const float &tex_xf,
                                        const float &tex_yf, const float &SX, const float &SY, const float &SZ, const float &billboard_mode,
                                        const float &X, const float &Y, const float &Z, const float &vsize, const float height)
{
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});

    tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});
    tvertices.emplace_back(t3d{tex_xf, tex_yf});

    vertices.emplace_back(v3d{X + vsize, Z, Y});
    vertices.emplace_back(v3d{X + vsize, Z, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z + height, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z + height, Y});

    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});

    normals.emplace_back(v3d{1.0f, 0.0f, 0.0f});
    normals.emplace_back(v3d{1.0f, 0.0f, 0.0f});
    normals.emplace_back(v3d{1.0f, 0.0f, 0.0f});
    normals.emplace_back(v3d{1.0f, 0.0f, 0.0f});
}

void render_block::primitives_cube_north(const float &light_r, const float &light_g, const float &light_b, const float &tex_xf,
                                         const float &tex_yf, const float &SX, const float &SY, const float &SZ, const float &billboard_mode,
                                         const float &X, const float &Y, const float &Z, const float &vsize, const float height)
{
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
    vertices.emplace_back(v3d{X + vsize, Z + height, Y});
    vertices.emplace_back(v3d{X, Z + height, Y});

    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});

    normals.emplace_back(v3d{0.0f, 0.0f, -1.0f});
    normals.emplace_back(v3d{0.0f, 0.0f, -1.0f});
    normals.emplace_back(v3d{0.0f, 0.0f, -1.0f});
    normals.emplace_back(v3d{0.0f, 0.0f, -1.0f});
}

void render_block::primitives_cube_south(const float &light_r, const float &light_g, const float &light_b, const float &tex_xf,
                                         const float &tex_yf, const float &SX, const float &SY, const float &SZ, const float &billboard_mode,
                                         const float &X, const float &Y, const float &Z, const float &vsize, const float height)
{
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});

    tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});
    tvertices.emplace_back(t3d{tex_xf, tex_yf});

    vertices.emplace_back(v3d{X, Z, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z + height, Y + vsize});
    vertices.emplace_back(v3d{X, Z + height, Y + vsize});

    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});

    normals.emplace_back(v3d{0.0f, 0.0f, 1.0f});
    normals.emplace_back(v3d{0.0f, 0.0f, 1.0f});
    normals.emplace_back(v3d{0.0f, 0.0f, 1.0f});
    normals.emplace_back(v3d{0.0f, 0.0f, 1.0f});
}

void render_block::primitives_cube_ceiling(const float &light_r, const float &light_g, const float &light_b, const float &tex_xf,
                                         const float &tex_yf, const float &SX, const float &SY, const float &SZ, const float &billboard_mode,
                                         const float &X, const float &Y, const float &Z, const float &vsize, const float height)
{
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});

    tvertices.emplace_back(t3d{tex_xf, tex_yf});
    tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
    tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

    vertices.emplace_back(v3d{X,        Z+height, Y});
    vertices.emplace_back(v3d{X,        Z+height, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z+height, Y + vsize});
    vertices.emplace_back(v3d{X + vsize, Z+height, Y});

    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});
    screen_index.emplace_back(v4d{SX,SY,SZ,billboard_mode});

    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
}

void render_block::add_floor(const int &x, const int &y, const int &z, const rltk::vchar &c, const GLfloat &billboard_mode)
{
    float light_r, light_g, light_b;
    std::tie(light_r, light_g, light_b) = altitude_color_bias(c, z);

    float X,Y,Z,vsize,tex_x,tex_y,tex_xf,tex_yf;
    std::tie(X,Y,Z,vsize,tex_x,tex_y,tex_xf,tex_yf) = size_calcs(c);

    float SX, SY, SZ;
    std::tie(SX, SY, SZ) = screenspace_calc(x, y, z);

    primitives_floor(light_r, light_g, light_b, tex_xf, tex_yf, SX, SY, SZ, billboard_mode);
}

void render_block::add_standup(const int &x, const int &y, const int &z, const rltk::vchar &c, const GLfloat &billboard_mode)
{
    float light_r, light_g, light_b;
    std::tie(light_r, light_g, light_b) = altitude_color_bias(c, z);

    float X,Y,Z,vsize,tex_x,tex_y,tex_xf,tex_yf;
    std::tie(X,Y,Z,vsize,tex_x,tex_y,tex_xf,tex_yf) = size_calcs(c);

    float SX, SY, SZ;
    std::tie(SX, SY, SZ) = screenspace_calc(x, y, z);

    // Floor
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});
    cvertices.emplace_back(c3d{light_r, light_g, light_b});

    if (camera->camera_mode == TOP_DOWN) {
        // Paste to the floor
        tvertices.emplace_back(t3d{tex_xf, tex_yf});
        tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
        tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
        tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});

        vertices.emplace_back( v3d{ -0.5f, -0.5f, -0.5f } );
        vertices.emplace_back( v3d{ -0.5f, -0.5f,  0.5f } );
        vertices.emplace_back( v3d{  0.5f, -0.5f,  0.5f } );
        vertices.emplace_back( v3d{  0.5f, -0.5f, -0.5f } );
    } else {
        // Vertical
        tvertices.emplace_back(t3d{tex_xf, tex_yf + tex_ysize});
        tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf + tex_ysize});
        tvertices.emplace_back(t3d{tex_xf + tex_xsize, tex_yf});
        tvertices.emplace_back(t3d{tex_xf, tex_yf});

        vertices.emplace_back(v3d{-0.5f, -0.5f, 0.5f});
        vertices.emplace_back(v3d{ 0.5f, -0.5f, 0.5f});
        vertices.emplace_back(v3d{ 0.5f, 0.5f, 0.5f});
        vertices.emplace_back(v3d{-0.5f, 0.5f, 0.5f});
    }

    if (camera->camera_mode == TOP_DOWN) {
        screen_index.emplace_back(v4d{SX, SY, SZ, 0.0f});
        screen_index.emplace_back(v4d{SX, SY, SZ, 0.0f});
        screen_index.emplace_back(v4d{SX, SY, SZ, 0.0f});
        screen_index.emplace_back(v4d{SX, SY, SZ, 0.0f});
    } else {
        screen_index.emplace_back(v4d{SX, SY, SZ, 1.0f});
        screen_index.emplace_back(v4d{SX, SY, SZ, 1.0f});
        screen_index.emplace_back(v4d{SX, SY, SZ, 1.0f});
        screen_index.emplace_back(v4d{SX, SY, SZ, 1.0f});
    }

    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
    normals.emplace_back(v3d{0.0f, 1.0f, 0.0f});
}

void render_block::add_decal(const int &x, const int &y, const int &z, const rltk::vchar &c, const GLfloat &billboard_mode)
{
    float light_r, light_g, light_b;
    std::tie(light_r, light_g, light_b) = altitude_color_bias(c, z);

    float X,Y,Z,vsize,tex_x,tex_y,tex_xf,tex_yf;
    std::tie(X,Y,Z,vsize,tex_x,tex_y,tex_xf,tex_yf) = size_calcs(c);

    float SX, SY, SZ;
    std::tie(SX, SY, SZ) = screenspace_calc(x, y, z);

    primitives_floor(light_r, light_g, light_b, tex_xf, tex_yf, SX, SY, SZ, billboard_mode);
}

void render_block::add_cube(const int &x, const int &y, const int &z, const rltk::vchar &c, const GLfloat &billboard_mode) {
    float light_r, light_g, light_b;
    std::tie(light_r, light_g, light_b) = altitude_color_bias(c, z);

    float X,Y,Z,vsize,tex_x,tex_y,tex_xf,tex_yf;
    std::tie(X,Y,Z,vsize,tex_x,tex_y,tex_xf,tex_yf) = size_calcs(c);

    float SX, SY, SZ;
    std::tie(SX, SY, SZ) = screenspace_calc(x, y, z);

    primitives_floor(light_r, light_g, light_b, tex_xf, tex_yf, SX, SY, SZ, billboard_mode);
    primitives_cube_left(light_r, light_g, light_b, tex_xf, tex_yf, SX, SY, SZ, billboard_mode, X, Y, Z, vsize, 0.9f);
    primitives_cube_right(light_r, light_g, light_b, tex_xf, tex_yf, SX, SY, SZ, billboard_mode, X, Y, Z, vsize, 0.9f);
    primitives_cube_north(light_r, light_g, light_b, tex_xf, tex_yf, SX, SY, SZ, billboard_mode, X, Y, Z, vsize, 0.9f);
    primitives_cube_south(light_r, light_g, light_b, tex_xf, tex_yf, SX, SY, SZ, billboard_mode, X, Y, Z, vsize, 0.9f);
    primitives_cube_ceiling(light_r, light_g, light_b, tex_xf, tex_yf, SX, SY, SZ, billboard_mode, X, Y, Z, vsize, 0.9f);
}

void render_block::add_fractional_height_cube(const int &x, const int &y, const int &z, const rltk::vchar &c, const float &height, const GLfloat &billboard_mode)
{
    float light_r, light_g, light_b;
    std::tie(light_r, light_g, light_b) = altitude_color_bias(c, z);

    float X,Y,Z,vsize,tex_x,tex_y,tex_xf,tex_yf;
    std::tie(X,Y,Z,vsize,tex_x,tex_y,tex_xf,tex_yf) = size_calcs(c);

    float SX, SY, SZ;
    std::tie(SX, SY, SZ) = screenspace_calc(x, y, z);

    primitives_floor(light_r, light_g, light_b, tex_xf, tex_yf, SX, SY, SZ, billboard_mode);
    primitives_cube_left(light_r, light_g, light_b, tex_xf, tex_yf, SX, SY, SZ, billboard_mode, X, Y, Z, vsize, height);
    primitives_cube_right(light_r, light_g, light_b, tex_xf, tex_yf, SX, SY, SZ, billboard_mode, X, Y, Z, vsize, height);
    primitives_cube_north(light_r, light_g, light_b, tex_xf, tex_yf, SX, SY, SZ, billboard_mode, X, Y, Z, vsize, height);
    primitives_cube_south(light_r, light_g, light_b, tex_xf, tex_yf, SX, SY, SZ, billboard_mode, X, Y, Z, vsize, height);
    primitives_cube_ceiling(light_r, light_g, light_b, tex_xf, tex_yf, SX, SY, SZ, billboard_mode, X, Y, Z, vsize, height);
}

void render_block::render(const GLuint &program_id) const noexcept {
    //assert(vertices.size() == billboard.size());

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, &cvertices[0]);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, &tvertices[0]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, &normals[0]);

    GLint index_pos = glGetAttribLocation(program_id, "screen_index");
    glEnableVertexAttribArray(index_pos);
    glVertexAttribPointer(index_pos, 4, GL_FLOAT, 0, 0, &screen_index[0]);

    glDrawArrays(GL_QUADS, 0, vertices.size());

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
