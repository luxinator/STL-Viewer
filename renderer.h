//
// Created by lucas on 4/15/16.
//

#ifndef STLVIEWER_RENDERER_H
#define STLVIEWER_RENDERER_H

#include <vector>

#include "STLData.h"


//GL Headers
//!!! KEEP IN THIS ORDER !!!:
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"



class renderer {


    GLuint VBO, VAO, shaderProgram;

//    std::vector<triangle *> triangles;

    GLfloat * vertices;
    GLsizei nrOfVertices;
    const GLuint id;
    const GLuint vertexdepth;

    float boundingBox[6];

public:

    renderer(unsigned int renderNr, unsigned int vertexDepth = 3);
    void loadTriangles(std::vector<triangle *> triangles);
    void update();
    void draw();

    ~renderer();


    const unsigned int getId() const {
        return (unsigned int) id;
    }

    const float *getBoundingBox() const {
        return boundingBox;
    }
};


#endif //STLVIEWER_RENDERER_H
