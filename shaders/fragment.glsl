#version 330 core
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec4 color;

void main() {
    color = vec4(objectColor * lightColor, 1.0f);

}