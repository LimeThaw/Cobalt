//Sample program for Project Cobalt

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "w3d.h"

int main(){

    //init framerate counting
    int fps=0, fpsc=glfwGetTime();

    //init GLFW
    if(!glfwInit()){
        std::cerr << "-Failed to initialize GLFW\n";
        return -1;
    }else{
        std::cout << "-Initialized GLFW\n";
    }

    //Give GLFW some info
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //init window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGLTest", NULL, 0);
    if(window==NULL){
        std::cerr << "-Failed to open window\n";
        return -1;
    }else{
        std::cout << "-Opened window\n";
    }
    glfwMakeContextCurrent(window);

    //init GLEW
    if(glewInit()!=GLEW_OK){
        std::cerr << "-Failed to initialize GLEW\n";
        return -1;
    }else{
        std::cout << "-Initialized GLEW\n\n";
    }

     glGetError();

    //setup some OpneGL functions
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //Load shaders and textures
    unsigned int shader = load_global_shader("vertexShader.glsl", "textureFragmentShader.glsl");
    unsigned int solidShader = load_global_shader("vertexShader.glsl", "solidFragmentShader.glsl");
    unsigned int mapMat = create_material(shader, load_global_texture("testmapTex_small.png"));
    unsigned int robotMat = create_material(solidShader);

    //Load objects, give them materials and place them in world
    scene my_world;
    node* map_node = new node("testmap.obj");
    map_node->set_material(mapMat);
    my_world.append_node(map_node);
    node* robot_node = new node("Robot.obj");
    robot_node->set_material(robotMat);
    robot_node->set_scale(0.3f);
    my_world.append_node(robot_node);

    //Unused zoom function
    float zoom = 2.0f;
    my_world.get_parent_node()->set_scale(zoom);

    //load camera
    set_active_camera(create_camera(glm::vec3(0, 10, 5), glm::vec3(0, 1, 0)));

    //Setup rotation and location
    float roty = 0.0f;
    float rotx = 0.0f;
    float posx = 0.0f;
    float posz = 0.0f;

    //Setup key capturing
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

    //loop
    bool quit = false;
    while(!quit && glfwGetKey(window, GLFW_KEY_ESCAPE)!=GLFW_PRESS){

        //count framerate
        if(glfwGetTime()-fpsc>=1.0f){
            std::clog << "-FPS: " << fps << '\n';
            fps=0;
            fpsc=glfwGetTime();
        }else{
            fps+=1;
        }

        //Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //move world
        if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS)posx+=0.01;
        if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS)posx-=0.01;
        if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS)posz+=0.01;
        if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS)posz-=0.01;
        if(glfwGetKey(window, GLFW_KEY_UP)==GLFW_PRESS);

        //Position and render world
        my_world.get_parent_node()->set_orientation(0, roty, 0);
        my_world.get_parent_node()->place(posx, -5, posz);
        my_world.render();

        //Update window and events
        glfwSwapBuffers(window);
        glfwPollEvents();

        //check for OpenGL errors
        GLenum error = glGetError();
        if(error != 0){
            std::cerr << "! Detected GL Error: " << error << '\n';
            quit = true;
        }
    }

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

}
