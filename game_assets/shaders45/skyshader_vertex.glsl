#version 430

layout (location = 0) in vec3 aPosition;

out vec3 vPosition;

void main() {
    gl_Position = vec4(aPosition, 1.0);
    vPosition = aPosition;
}
