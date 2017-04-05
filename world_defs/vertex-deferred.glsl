#version 120

uniform vec3 light_position;
uniform vec3 light_ambient;
uniform vec3 light_diffuse;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
varying vec3 normal, ambient;
varying vec4 diffuse;
varying vec3 light_pos_frag;
varying vec3 v;
attribute vec4 screen_index;
varying vec3 si;

mat4 translate(float x, float y, float z){
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x,   y,   z,   1.0)
    );
}

// Outdoor rendering
void main()
{
    // Are we in billboard mode?
    bool billboard_mode;
    if (screen_index.w != 1.0) {
        billboard_mode = false;
    } else {
        billboard_mode = true;
    }

    // Calculate world position from the screen index; note that Y and Z are flipped
    float world_x = screen_index.x * 255.0;
    float world_y = screen_index.z * 255.0;
    float world_z = screen_index.y * 255.0;

    // Transforming The Vertex
    if (billboard_mode) {
        // We want to rotate to face the camera
        mat4 ModelView = view_matrix * translate(world_x, world_y, world_z);

        // Column 0:
        ModelView[0][0] = 1;
        ModelView[0][1] = 0;
        ModelView[0][2] = 0;

        // Column 1:
        ModelView[1][0] = 0;
        ModelView[1][1] = 1;
        ModelView[1][2] = 0;

        // Column 2:
        ModelView[2][0] = 0;
        ModelView[2][1] = 0;
        ModelView[2][2] = 1;

        vec4 position = gl_Vertex; // At this point we're in -0.5 to 0.5 space
        gl_Position = projection_matrix * (ModelView * position);
    } else {
        // Simple transform to world-space
        vec4 position = gl_Vertex;
        position.x += world_x;
        position.y += world_y;
        position.z += world_z;
        gl_Position = projection_matrix * (view_matrix * position);
    }

    vec4 position = gl_Vertex;
    position.x += world_x;
    position.y += world_y;
    position.z += world_z;

    // Transform the normal and normalize (heh) it
    normal = gl_Normal;

    // Now we get the light direction (in world space)
    light_pos_frag = light_position;
    v = vec3(position);

    // Now we get the dot product between the light and the vertex
    diffuse = vec4(light_diffuse, 1.0f) * gl_Color;

    // Pass the color through because we use it for colored textures
    gl_FrontColor = gl_Color;
    ambient = light_ambient;
    si = screen_index.xyz;

    // Project the texture
    gl_TexCoord[0] = gl_MultiTexCoord0;
}