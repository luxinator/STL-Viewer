#version 330 core
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;

in vec3 Normal;
in vec3 FragPos;
out vec4 color;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 1.0f);
    vec3 diffuse = diff * lightColor;
    vec3 result = (diffuse) * objectColor;
    color = vec4(result, 1.0f);

}