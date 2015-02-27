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

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasingglfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
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

    //init ZBuffer and textures
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

    //load mesh
    /*mesh monkey;
    monkey.load("example.obj", "marble.tga");*/

    //load node
    /*mesh desk;
    desk.load_model("Schreibtisch.obj");*/

    //Load texture shader
    /*shader texture_shader;
    texture_shader.load_shader("vertexShader.glsl", "solidFragmentShader.glsl");
    texture_shader.use();*/

    unsigned int shader = load_global_shader("vertexShader.glsl", "textureFragmentShader.glsl");
    unsigned int solidShader = load_global_shader("vertexShader.glsl", "solidFragmentShader.glsl");
    unsigned int mapMat = create_material(shader, load_global_texture("testmapTex_small.png"));
    //unsigned int houseMat = create_material(shader, load_global_texture("houseTex.png"));
    unsigned int robotMat = create_material(solidShader);

    scene my_world;
    node* map_node = new node("testmap.obj");
    map_node->set_material(mapMat);
    my_world.append_node(map_node);
    /*node* house_node = new node("smallHouse.obj");
    house_node->set_material(houseMat);
    my_world.append_node(house_node);*/
    node* robot_node = new node("Robot.obj");
    robot_node->set_material(robotMat);
    robot_node->set_scale(0.3f);
    my_world.append_node(robot_node);

    float zoom = 2.0f;
    //my_world.get_parent_node()->set_material(solidShader);
    my_world.get_parent_node()->set_scale(zoom);

    //Load solid shader
    /*shader solid_shader;
    solid_shader.load_shader("vertexShader.glsl", "solidFragmentShader.glsl");
*/
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

        //rotate triangle
        //roty += 0.05f;
        rotx += 0.025f;

        //move world
        if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS)posx+=0.01;
        if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS)posx-=0.01;
        if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS)posz+=0.01;
        if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS)posz-=0.01;
        if(glfwGetKey(window, GLFW_KEY_UP)==GLFW_PRESS);


        //house_node->set_scale(rotx/100);

        /*monkey.place(0, 3, 0);
        monkey.set_orientation(0, roty, 0);
        monkey.set_scale(0.5f);
        monkey.render();*/

        /*desk.place(rotx/20-10, 0, 0);
        desk.set_orientation(0, rotx*2, 0);
        desk.render();*/

        my_world.get_parent_node()->set_orientation(0, roty, 0);
        my_world.get_parent_node()->place(posx, -5, posz);
        my_world.render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        GLenum error = glGetError();
        if(error != 0){
            std::cerr << "! Detected GL Error: " << error << '\n';
            quit = true;
        }
    }

        // Close OpenGL window and terminate GLFW
        glfwTerminate();

}
