//
// Created by lucas on 5/9/16.
//

#ifndef MICROMODELGEN_STL_SHADERS_H_H
#define MICROMODELGEN_STL_SHADERS_H_H


const char * vertexSrc = R"(#version 330 core
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
)";

const char * fragmentSrc = R"(#version 330 core

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;

in vec3 Normal;
in vec3 FragPos;
out vec4 color;

void main() {

    //Ambient Light
    vec3 ambient = 0.1f * lightColor;

    //Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.1f);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    color = vec4(result, 1.0f);

}
)";

#endif //MICROMODELGEN_STL_SHADERS_H_H
