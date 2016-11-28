//
// Created by lucas on 4/15/16.
//

#include "renderer.h"
#include "Camera.h"
#include "GLError.h"
#include "shaders.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
 * UTILITY FUNCTIONS:
 */

float findMax(float *list, size_t size, size_t start = 0, size_t stride = 0) {

    float val = FLT_MIN;

    for (size_t i = start; i < size - stride; i++) {
        if (list[i * stride] > val)
            val = list[i * stride];
    }
    return val;
}

float findMin(float *list, size_t size, size_t start = 0, size_t stride = 0) {

    float val = FLT_MAX;

    for (size_t i = start; i < size - stride; i++) {
        if (list[i * stride] < val)
            val = list[i * stride];
    }
    return val;
}


bool checkShaderCompilation(GLuint shaderObject) {
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderObject, 512, NULL, infoLog);
        fprintf(stderr, "SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
        return false;
    }
    return true;
}

bool checkShaderLink(GLuint shaderProgram) {
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "SHADER::VERTEX::LINKING_FAILED\n%s\n", infoLog);
        return false;
    }
    return true;
}

/**
 *  Class functions
 */

renderer::renderer(unsigned int id, float objectColorR, float objectColorG, float objectColorB, unsigned int vertexDepth, const char * vertexPath, const char * fragmentPath) : id(id), vertexdepth(vertexDepth) {

    // --------- Read shaders Sources
    printf("Building Renderer\n");

    // 1. Retrieve the vertex/fragment source code from filePath
//    std::string vertexCode;
//    std::string fragmentCode;
//    std::ifstream vShaderFile;
//    std::ifstream fShaderFile;
//    // ensures ifstream objects can throw exceptions:
//    vShaderFile.exceptions(std::ifstream::badbit);
//    fShaderFile.exceptions(std::ifstream::badbit);
//    try {
//        // Open files
//        vShaderFile.open(vertexPath);
//        fShaderFile.open(fragmentPath);
//        std::stringstream vShaderStream, fShaderStream;
//        // Read file's buffer contents into streams
//        vShaderStream << vShaderFile.rdbuf();
//        fShaderStream << fShaderFile.rdbuf();
//        // close file handlers
//        vShaderFile.close();
//        fShaderFile.close();
//        // Convert stream into string
//        vertexCode = vShaderStream.str();
//        fragmentCode = fShaderStream.str();
//    }
//    catch (std::ifstream::failure e) {
//        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
//        std::abort();
//    }

    const GLchar *vertexShaderSrc = vertexSrc;
    const GLchar *fragmentShaderSrc = fragmentSrc;

    // ---------- Shaders:
    //Load the Vertex Shader, i.e. the vertex transformations
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);
    if (!checkShaderCompilation(vertexShader)) {
        std::abort();
    }

    //Load the Fragment Shader, i.e. pretty color maker:
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);

    if (!checkShaderCompilation(fragmentShader)) {
        std::abort();
    }

    //Link the Objects to a program:
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    if (!checkShaderLink(shaderProgram)) {
        std::abort();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Init variables, easy to check, NONE should be zero upon draw!
    vertices = nullptr;
    nrOfVertices = 0;
    VBO = 0;
    VAO = 0;

    for (size_t i = 0; i < 6; i++) {
        boundingBox[i] = 0.0f;
    }

    check_gl_error();

    this->objectColor[0] = objectColorR;
    this->objectColor[1] = objectColorG;
    this->objectColor[2] = objectColorB;

    printf("Renderer: Created Succesfully\n");

}

void renderer::draw(glm::mat4 transform, float lightingColor[3], glm::vec3 lightPos) {

    if (!VBO || !VAO || !shaderProgram || !vertices) {
        fprintf(stderr, "Renderer: Something went wrong!!!\n");
        fprintf(stderr, "VBO: %i\nVAO: %i\nshaderProgram: %i\nvertices: %p\n", VBO, VAO, shaderProgram, vertices);
        std::abort();
    }
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    //Pass the transformation from the window, ie zoom, location of camera etc.
    GLint transform_vrt = glGetUniformLocation(shaderProgram, "transform_vrt");
    glUniformMatrix4fv(transform_vrt, 1, GL_FALSE, glm::value_ptr(transform));

    //Pass the LightColor
    GLint lightColor = glGetUniformLocation(shaderProgram, "lightColor");
    glUniform3f(lightColor, lightingColor[0], lightingColor[1], lightingColor[2]);

    //Pass the ObjectColor
    GLint objColor = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform3f(objColor, objectColor[0], objectColor[1], objectColor[2]);

    //Pass the LightLocation
    GLint lightLoc = glGetUniformLocation(shaderProgram, "lightPos");
    glUniform3f(lightLoc, lightPos.x, lightPos.y, lightPos.z);

    glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);

    //Free the bind to the Vertex, now no one can access it and thus fuck it up!
    glBindVertexArray(0);
    check_gl_error();
}


void renderer::update() {

    //If any, empty new buffers
    if(VAO){
         glDeleteVertexArrays(1, &VAO);
     }
    if(VBO){
        glDeleteBuffers(1, &VBO);
    }
    //And create new ones
    //Vertex Array Object
    glGenVertexArrays(1, &VAO);
    //Vertex Buffer Object:
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    //Activate it
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //load our vertices to the GPU:
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * nrOfVertices * vertexdepth, vertices, GL_STATIC_DRAW);

    //Tell openGL how to read our Vertices:
    //location 0 in vertex shader can be read as follows:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    //location 1 in vertex shader can be read as follows:
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1); 

    glBindVertexArray(0);
    printf("Renderer: Wrote %.0f kB of data to GPU\n", (float) (6 * sizeof(GLfloat) * (float) nrOfVertices) / 1000);

}

void renderer::loadTriangles(std::vector<triangle *> triangles) {

    //Do a size Check in futher!
    //We need three points(vertices) per triangle
    this->nrOfVertices = (GLsizei) triangles.size() * 3;

    //We need three cooirdinates per vertex and a normal
    this->vertices = new GLfloat[6 * nrOfVertices];

    for (GLsizei i = 0; i < (GLsizei) triangles.size(); i++) {
        triangle *t = triangles[i];

        GLsizei j = i * 18;
        this->vertices[j] = t->v1[0];
        this->vertices[j + 1] = t->v1[1];
        this->vertices[j + 2] = t->v1[2];

        this->vertices[j + 3] = t->ni;
        this->vertices[j + 4] = t->nj;
        this->vertices[j + 5] = t->nk;

        this->vertices[j + 6] = t->v2[0];
        this->vertices[j + 7] = t->v2[1];
        this->vertices[j + 8] = t->v2[2];

        this->vertices[j + 9] = t->ni;
        this->vertices[j + 10] = t->nj;
        this->vertices[j + 11] = t->nk;

        this->vertices[j + 12] = t->v3[0];
        this->vertices[j + 13] = t->v3[1];
        this->vertices[j + 14] = t->v3[2];

        this->vertices[j + 15] = t->ni;
        this->vertices[j + 16] = t->nj;
        this->vertices[j + 17] = t->nk;
    }

    boundingBox[0] = findMin(vertices, nrOfVertices, 0, 3);
    boundingBox[1] = findMax(vertices, nrOfVertices, 0, 3);

    boundingBox[2] = findMin(vertices, nrOfVertices, 1, 3);
    boundingBox[3] = findMax(vertices, nrOfVertices, 1, 3);

    boundingBox[4] = findMin(vertices, nrOfVertices, 2, 3);
    boundingBox[5] = findMax(vertices, nrOfVertices, 2, 3);


    printf("Renderer: Loaded Vertices [%i]\n", nrOfVertices);
    printf("\tMin\t\t\t\tMax\n");
    printf("x\t%f\t\t%f\n", boundingBox[0], boundingBox[1]);
    printf("y\t%f\t\t%f\n", boundingBox[2], boundingBox[3]);
    printf("z\t%f\t\t%f\n", boundingBox[4], boundingBox[5]);
    this->update();
}

renderer::~renderer() {

    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    delete[] vertices;
    printf("Renderer: Destroyed\n");
}
