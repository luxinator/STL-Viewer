//
// Created by lucas on 4/15/16.
//

#include "window_GL.h"

#include <iostream>
#include <cstdio>
#include <cstring>





window_GL::window_GL(const int width, const int height, const char *title) :width(width), height(height), title(title) {


    //Starting GLFW:
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);



    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr)
    {
       fprintf(stderr,"Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    //Make GLEW do its magic:
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr,"Failed to initialize GLEW");
        fprintf(stderr, "%s\n", strerror(errno));
        return;
    }
    glViewport(0, 0, width, height);

    printf("Window: Created Succesfully\n");

}

void window_GL::loop(){

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for(size_t i = 0; i < renderers.size(); i++){
            renderers[i]->draw();
        }

        glfwSwapBuffers(window);
    }
}

window_GL::~window_GL() {

    for(size_t i = 0; i < renderers.size(); i++){
        renderers[i]->~renderer();
    }
    glfwTerminate();
    printf("Window: Destroyed\n");

}

size_t window_GL::registerRenderer(renderer *r) {

    renderers.push_back(r);
    printf("Window: Registerd Renderer[%i]\n", r->getId());
    return renderers.size();
}