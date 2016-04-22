//
// Created by lucas on 4/15/16.
//

#include "window_GL.h"

#include <iostream>
#include <cstdio>
#include <cstring>

//Interaction with user stuff:
Camera *camera;
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLboolean polygon;

//Prototypes
void Do_Movement();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


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

    // Get a Camera:
    camera = new Camera(glm::vec3(0.0f,0.0f,1.0f));

    //Register callbacks:
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    polygon = GL_FALSE;

    lightColor[0] = 1.0f;
    lightColor[1] = 1.0f;
    lightColor[2] = 1.0f;


    printf("Window: Created Successfully\n");

}

void window_GL::loop(){

    while(!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Get Input:
        glfwPollEvents();
        Do_Movement();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Create camera transformation
        glm::mat4 view;
        view = camera->GetViewMatrix();
        glm::mat4 projection;
        projection = glm::perspective(camera->Zoom, (float)width/(float)height, 0.1f, 1000.0f);

        projection = projection * view;


        for(size_t i = 0; i < renderers.size(); i++){
            renderers[i]->draw(projection, lightColor,camera->Position);
        }

        glfwSwapBuffers(window);

        if(polygon == GL_TRUE){
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
        } else
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
        }
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
    const float *box = r->getBoundingBox();
    camera->Position.x = (box[0] + box[1])/2.0f;
    camera->Position.y = (box[2] + box[3])/2.0f;
    camera->Position.z = (box[4] + box[5])/2.0f * 1.5f;

    return renderers.size();
}

void Do_Movement() {
    // Camera controls
    if (keys[GLFW_KEY_W])
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera->ProcessKeyboard(RIGHT, deltaTime);
    if (keys[GLFW_KEY_P]) {
        if(polygon == GL_TRUE)
            polygon = GL_FALSE;
        else
            polygon = GL_TRUE;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

//    printf("%i\n",key);
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;
    }
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

    lastX = xpos;
    lastY = ypos;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        camera->ProcessLeftMouseMovement(xoffset, yoffset);
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        camera->ProcessRightMouseMovement(xoffset, yoffset);
    }
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(yoffset);
}

