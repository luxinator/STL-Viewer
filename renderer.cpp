//
// Created by lucas on 4/15/16.
//

#include "renderer.h"


//Shaders:
const GLchar *vertexShaderSrc = "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "}\0";

const GLchar *fragmentShaderSrc = "#version 330 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";


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


renderer::renderer(unsigned int id, unsigned int vertexDepth) : id(id), vertexdepth(vertexDepth) {

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


    printf("Renderer: Created Succesfully\n");

}

void renderer::draw() {

    if (!VBO || !VAO || !shaderProgram || !vertices) {
        fprintf(stderr, "Renderer: Something went wrong!!!\n");
        std::abort();
    }
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);

    //Free the bind to the Vertex, now no one can access it and thus fuck it up!
    glBindVertexArray(0);

}

void renderer::update() {

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
    glVertexAttribPointer(id, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(id);

    glBindVertexArray(0);

    printf("Renderer: Wrote data %f kB to GPU\n", (float) (3 * sizeof(GLfloat) * (float) nrOfVertices) / 1000);

}

void renderer::loadTriangles(std::vector<triangle *> triangles) {

    //Do a size Check in futher!
    //We need three points(vertices) per triangle
    this->nrOfVertices = (GLsizei) triangles.size() * 3;

    //We need three cooirdinates per vertex
    this->vertices = new GLfloat[3 * nrOfVertices];

    for (GLsizei i = 0; i < (GLsizei) triangles.size(); i++) {
        triangle *t = triangles[i];

        GLsizei j = i * 9;
        this->vertices[j] = t->v1[0];
        this->vertices[j + 1] = t->v1[1];
        this->vertices[j + 2] = t->v1[2];

        this->vertices[j + 3] = t->v2[0];
        this->vertices[j + 4] = t->v2[1];
        this->vertices[j + 5] = t->v2[2];

        this->vertices[j + 6] = t->v3[0];
        this->vertices[j + 7] = t->v3[1];
        this->vertices[j + 8] = t->v3[2];

    }

    boundingBox[0] = findMin(vertices, nrOfVertices, 0, 3);
    boundingBox[1] = findMax(vertices, nrOfVertices, 0, 3);

    boundingBox[2] = findMin(vertices, nrOfVertices, 1, 3);
    boundingBox[3] = findMax(vertices, nrOfVertices, 1, 3);

    boundingBox[4] = findMin(vertices, nrOfVertices, 2, 3);
    boundingBox[5] = findMax(vertices, nrOfVertices, 2, 3);


    printf("Renderer: Loaded Vertices [%i]\n", nrOfVertices);
    printf("\tMin\t\t\tMax\n");
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
