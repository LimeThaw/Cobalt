//Sample program for Project Cobalt

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <math.h>

#include "cobalt.h"
#include "simple_render_pass.h"

int main() {

    //init framerate counting
    int fps = 0, fpsc = glfwGetTime();
    
    const std::string model_dir = "./demo/res/models/";
    const std::string shader_dir = "./demo/res/shaders/";
    const std::string texture_dir = "./demo/res/textures/";

    //init GLFW
    if(!glfwInit()) {
        std::cerr << "-Failed to initialize GLFW\n";
        return -1;
    } else {
        std::cout << "-Initialized GLFW\n";
    }

    //Give GLFW some info
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    //init window
    GLFWwindow *window = glfwCreateWindow(1280, 640, "OpenGLTest", NULL, 0);
    if(window == NULL) {
        std::cerr << "-Failed to open window\n";
        return -1;
    } else {
        std::cout << "-Opened window\n";
    }
    glfwMakeContextCurrent(window);

    //init GLEW
    if(glewInit() != GLEW_OK) {
        std::cerr << "-Failed to initialize GLEW\n";
        return -1;
    } else {
        std::cout << "-Initialized GLEW\n\n";
    }
    clear_gl_error();

    //setup some OpneGL functions
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);



    //Load shaders and textures
    unsigned int shader = load_global_shader(shader_dir + "vertexShader.glsl", shader_dir +  "textureFragmentShader.glsl");
    unsigned int untextured_shader = load_global_shader(shader_dir + "vertexShader.glsl", shader_dir + "untexturedFragmentShader.glsl");
    unsigned int normal_shader = load_global_shader(shader_dir + "vertexShader.glsl", shader_dir + "normalFragmentShader.glsl");

    unsigned int map_mat = create_material(new texture_link(texture_dir + "testmapTex_small.png", "color_map"));
    unsigned int robot_mat = create_material();
    unsigned int monkey_mat = create_material();
    add_texture(monkey_mat, new texture_link(texture_dir + "dirt.jpeg", "color_map"));
    add_texture(monkey_mat, new texture_link(texture_dir + "dirt_normal.png", "normal_map"));

    simple_render_pass render_pass(shader, map_mat);
    simple_render_pass solid_render_pass(untextured_shader, robot_mat);
    simple_render_pass normal_render_pass(normal_shader, monkey_mat);

    auto screen = framebuffer::get_screen();

    //Load objects, give them materials and place them in world
    scene my_world;
    node *map_node = new node(model_dir + "testmap.obj");
    map_node->set_material(map_mat);
    my_world.append_node(map_node);
    node *robot_node = new node(model_dir + "Robot.obj");
    robot_node->set_material(robot_mat);
    robot_node->set_scale(0.3f);
    my_world.append_node(robot_node);
    node *monkey_node = new node(model_dir + "NormalExample.obj");
    monkey_node->set_material(monkey_mat);
    monkey_node->place(2, 0, 0);
    my_world.append_node(monkey_node);

    //Unused zoom function
    float zoom = 2.0f;
    my_world.get_parent_node()->set_scale(zoom);

    camera the_camera(glm::vec3(0, 10, 5), glm::vec3(0, 1, 0));
    std::vector<directional_light> directional_lights = { directional_light(glm::vec3(100, 200, 50), 3, glm::vec3(-2, 0.5, 2)) };
    std::vector<point_light> point_lights = { point_light(glm::vec3(255, 0, 0), 3, glm::vec3(0, 0.5, 1.5)),
                                              point_light(glm::vec3(0, 255, 0), 3, glm::vec3(0, -0.5, -1.5)),
                                              point_light(glm::vec3(0, 0, 255), 3, glm::vec3(1.0, 0, 0.5)),
    };
    glm::vec3 ambient_light_color = glm::vec3(0.3);

    //Setup rotation and location

    float posx = 0.0f;
    float posz = 0.0f;
    float intensity = 0.0f;

    //Setup key capturing
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

    //loop
    bool quit = false;
    while(!quit && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

        //count framerate
        if(glfwGetTime() - fpsc >= 1.0f) {
            std::clog << "-FPS: " << fps << '\n';

            fps = 0;
            fpsc = glfwGetTime();
        } else {
            fps += 1;
        }

        //Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //move world
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)posx += 0.01;
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)posx -= 0.01;
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)posz += 0.01;
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)posz -= 0.01;
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS);

        //change light intensity
        intensity += 0.001;
        directional_lights[0].set_intensity(std::abs(sin(intensity)));

        //Position and render world
        //my_world.get_parent_node()->set_orientation(0, roty, 0);
        //my_world.get_parent_node()->place(posx, -5, posz);
        the_camera.place(posx, 10, posz);
        //point_lights[0].set_position(glm::vec3(posx + 2, -3, posz + 2));
        render_pass.render(my_world, the_camera, directional_lights, point_lights, ambient_light_color, *screen);
        solid_render_pass.render(my_world, the_camera, directional_lights, point_lights, ambient_light_color, *screen);
        normal_render_pass.render(my_world, the_camera, directional_lights, point_lights, ambient_light_color, *screen);

        //Update window and events
        glfwSwapBuffers(window);
        glfwPollEvents();

        //check for OpenGL errors
        try {
            check_gl_error();
        } catch(gl_exception &e) {
            std::cerr << "! Detected GL Error: " << e.what() << '\n';
            quit = true;
        }

    }

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

}
