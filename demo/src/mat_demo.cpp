#include "cobalt.h"
#include "library/standard.h"
#include "simple_render_pass.h"

int main() {

    // Creating window object
    window win(1280, 620, "Cobalt Material Demo");

    //Creating scene and light(s)
    scene demo_world;
    std::vector<directional_light> d_lights = {
        directional_light(glm::vec3(100, 100, 100), 0.5, glm::vec3(-5, 5, -10))
    };
    std::vector<point_light> p_lights = {};

    // Loading the red sphere
    auto red_mat = std::make_shared<cs::std_material>(glm::vec3(2.0f, 0.5f, 0.5f), 0.5f, 1.0f);
    mesh *red_mesh = new mesh("./demo/res/models/mat_demo.obj");
    red_mesh->set_material(red_mat);
    red_mesh->place(-5, 0, 0);
    demo_world.append_node(red_mesh);

    // Loading the green sphere
    auto green_mat = std::make_shared<cs::std_material>(glm::vec3(0.5f, 1.0f, 0.5f), 0.5f, 1.0f);
    mesh *green_mesh = new mesh("./demo/res/models/mat_demo.obj");
    green_mesh->set_material(green_mat);
    demo_world.append_node(green_mesh);

    // Loading the blue sphere
    auto blue_mat = std::make_shared<cs::std_material>(glm::vec3(0.5f, 0.5f, 2.0f), 0.5f, 1.0f);
    mesh *blue_mesh = new mesh("./demo/res/models/mat_demo.obj");
    blue_mesh->set_material(blue_mat);
    blue_mesh->place(5, 0, 0);
    demo_world.append_node(blue_mesh);

    // Creating the render pass and camera
    cs::std_render_pass demo_render_pass;
    camera main_camera(glm::vec3(0, 5, -8), glm::vec3(0, 0, 0));

    // Introducing the roughness variable
    // Used to dynamically control the roughness of the spheres
    float roughness = 0.5f;

    // Declaring variables used for framerate counting
    float fpsc = glfwGetTime();
    int fps = 0;

    // Main loop
    bool quit = false;
    while(!quit && !win.key_pressed(GLFW_KEY_ESCAPE) && !win.should_close()) {

        // Checking for key strokes
        if(win.key_pressed(GLFW_KEY_W)) {
            if(roughness < 1.0f) roughness += 0.01f;
        }
        if(win.key_pressed(GLFW_KEY_S)) {
            if(roughness > 0.0f) roughness -= 0.01f;
        }

        // Setting roughness for the meshes
        red_mesh->get_material()->set_uniform("material_roughness", std::make_shared<float_uniform>(roughness));
        green_mesh->get_material()->set_uniform("material_roughness", std::make_shared<float_uniform>(roughness));
        blue_mesh->get_material()->set_uniform("material_roughness", std::make_shared<float_uniform>(roughness));

        // Rendering the scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        demo_render_pass.render(demo_world, main_camera, d_lights, p_lights);
        win.update();

        // Counting and displaying the current framerate
        if(glfwGetTime() - fpsc >= 1.0f) {
            std::clog << "-FPS: " << fps << "   \r";
            fps = 0;
            fpsc = glfwGetTime();
        } else {
            fps += 1;
        }

        // Checking for OpenGL errors (usually shader errors)
        try {
            check_gl_error();
        } catch(gl_exception &e) {
            std::cerr << "! Detected GL Error: " << e.what() << '\n';
            quit = true;
        }

    }

}

