#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 transform_vrt;

void main() {
    gl_Position = transform_vrt * vec4(position, 1.0f);
}