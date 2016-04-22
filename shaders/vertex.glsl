#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 transform_vrt;

out vec3 FragPos;
out vec3 Normal;

void main() {
    gl_Position = transform_vrt * vec4(position, 1.0f);
    FragPos = position;
    Normal = normal;
}