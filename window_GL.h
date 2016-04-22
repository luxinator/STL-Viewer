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
#include "Camera.h"


/*
 * Class resposible for the window, viewport and gui
 * Also for getting Events like clicks and key presses
 * Also holds all the renderers, meaning that multiple 3dobjects can co-exist
 * in one window.
 */
class window_GL {

    const int width, height;
    const char * title;

    GLFWwindow* window;
    std::vector<renderer *> renderers;

    float lightColor[3];

public:
    window_GL(const int width, const int height, const char * title);

    size_t registerRenderer(renderer *r);

    void loop();

    void setLightColor(float R, float G, float B){
        lightColor[0] = R;
        lightColor[1] = R;
        lightColor[2] = R;
    }
    /*
     * Return a COPY of the light color!
     */
    float * getLightColor(){
        float * n = new float[3];
        n[0] = lightColor[0];
        n[1] = lightColor[1];
        n[2] = lightColor[2];
        return n;
    }

    ~window_GL();

};


#endif //STLVIEWER_WINDOW_GL_H
