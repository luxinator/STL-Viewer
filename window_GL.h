//
// Created by lucas on 4/15/16.
//

#ifndef STLVIEWER_WINDOW_GL_H
#define STLVIEWER_WINDOW_GL_H

#include <vector>

//GL Headers
//!!! KEEP IN THIS ORDER !!!:
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "renderer.h"



/*
 * Class resposible for the window, viewport and gui
 * Also holds all the renderers, meaning that multiple 3dobjects can co-exist
 * in one window.
 */
class window_GL {

    const int width, height;
    const char * title;

    GLFWwindow* window;
    std::vector<renderer *> renderers;



public:
    window_GL(const int width, const int height, const char * title);

    size_t registerRenderer(renderer *r);

    void loop();


    ~window_GL();

};


#endif //STLVIEWER_WINDOW_GL_H
