//Sample program for Project Cobalt

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "cobalt.h"
#include "library/standard.h"
#include "simple_render_pass.h"

int main() {

	//Initialize window
	window win(1280, 640, "Cobalt Demo");
	
    //init framerate counting
    int fps = 0, fpsc = glfwGetTime();

    const std::string model_dir = "./demo/res/models/";
    const std::string shader_dir = "./demo/res/shaders/";
    const std::string texture_dir = "./demo/res/textures/";


    //Load shaders and textures
    auto textured_shader = std::make_shared<shader>(shader_dir + "vertexShader.glsl",
                                                    shader_dir + "textureFragmentShader.glsl");
    auto untextured_shader = std::make_shared<shader>(shader_dir + "vertexShader.glsl",
                                                      shader_dir + "untexturedFragmentShader.glsl");
    auto normal_shader = std::make_shared<shader>(shader_dir + "vertexShader.glsl",
                                                  shader_dir + "normalFragmentShader.glsl");
    auto mirror_shader = std::make_shared<shader>(shader_dir + "vertexShader.glsl",
                                                  shader_dir + "mirrorFragmentShader.glsl");
    auto parallax_shader = std::make_shared<shader>(shader_dir + "vertexShader.glsl",
                                                    shader_dir + "parallaxMappingFragmentShader.glsl");

    auto map_tex = std::make_shared<texture2d>(
            texture_cache::get_instance().get_texture_from_filename(texture_dir + "testmapTex_small.png"));
    auto map_mat = std::make_shared<material>(
            material::texture_bindings { material::texture_binding("color_map", map_tex) });

    auto robot_mat = std::make_shared<cs::std_material>();

    auto dirt_tex = std::make_shared<texture2d>(
            texture_cache::get_instance().get_texture_from_filename(texture_dir + "dirt.jpeg"));
    auto dirt_normal_tex = std::make_shared<texture2d>(
            texture_cache::get_instance().get_texture_from_filename(texture_dir + "dirt_normal.png"));
    auto monkey_mat = std::make_shared<material>(
            material::texture_bindings { material::texture_binding("color_map", dirt_tex),
                                         material::texture_binding("normal_map", dirt_normal_tex) });

    auto rtt_mat = std::make_shared<material>();

    auto mirror_mat = std::make_shared<material>();

    auto rockwall_tex = std::make_shared<texture2d>(
            texture_cache::get_instance().get_texture_from_filename(texture_dir + "rockwall.jpg"));
    auto rockwall_height_tex = std::make_shared<texture2d>(
            texture_cache::get_instance().get_texture_from_filename(texture_dir + "rockwall_height.bmp"));
    auto rockwall_normal_map = std::make_shared<texture2d>(
            texture_data_source::create_normals_from_height(
                    texture_cache::get_instance().get_texture_from_filename(texture_dir + "rockwall_height.bmp")));
    auto room_mat = std::make_shared<material>(
            material::texture_bindings {
                    material::texture_binding("color_map", rockwall_tex),
                    material::texture_binding("height_map", rockwall_height_tex),
                    material::texture_binding("normal_map", rockwall_normal_map)
            });

    simple_render_pass render_pass(textured_shader, map_mat);
    //simple_render_pass solid_render_pass(untextured_shader, robot_mat);
    cs::std_render_pass solid_render_pass;
    simple_render_pass normal_render_pass(normal_shader, monkey_mat);
    simple_render_pass rtt_pass(textured_shader, rtt_mat);
    simple_render_pass mirror_pass(mirror_shader, mirror_mat);
    simple_render_pass no_parallax_render_pass(normal_shader, room_mat);
    simple_render_pass parallax_render_pass(parallax_shader, room_mat);

    auto screen = framebuffer::get_screen();

    auto render_target_texture = std::make_shared<texture2d>(128, 128, GL_RGB8, GL_REPEAT,
                                                             GL_REPEAT,
                                                             GL_LINEAR,
                                                             GL_LINEAR);
    rtt_mat->add_texture("color_map", render_target_texture);
    auto depth_attachment = std::make_shared<renderbuffer_framebuffer_attachment>(GL_DEPTH_COMPONENT32, 128, 128);
    framebuffer offscreen_framebuffer(std::make_shared<texture_framebuffer_attachment>(render_target_texture),
                                      depth_attachment);

    auto mirror_cubemap = std::make_shared<cubemap>(128, 128, GL_RGB);
    mirror_mat->add_texture("environment", mirror_cubemap);
    std::vector<std::shared_ptr<framebuffer>> cubemap_framebuffers;
    for(GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X; face <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; ++face) {
        auto color_attachment = std::make_shared<texture_framebuffer_attachment>(mirror_cubemap, face);
        cubemap_framebuffers.push_back(std::make_shared<framebuffer>(color_attachment, depth_attachment));
    }

    glm::mat4 cubemap_projection = glm::perspective(90.0f, 1.0f, 0.1f, 100.0f);
    glm::vec3 mirror_position(3, 3, 2);
    std::vector<camera> cubemap_cameras {
            camera(glm::lookAt(mirror_position, mirror_position + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)),
                   cubemap_projection),
            camera(glm::lookAt(mirror_position, mirror_position + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
                   cubemap_projection),
            camera(glm::lookAt(mirror_position, mirror_position + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
                   cubemap_projection),
            camera(glm::lookAt(mirror_position, mirror_position + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)),
                   cubemap_projection),
            camera(glm::lookAt(mirror_position, mirror_position + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)),
                   cubemap_projection),
            camera(glm::lookAt(mirror_position, mirror_position + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)),
                   cubemap_projection)
    };

    //Load objects, give them materials and place them in world
    scene my_world;
    mesh *map_node = new mesh(model_dir + "testmap.obj");
    map_node->set_material(map_mat);
    my_world.append_mesh(map_node);
    node *robot_node = new node(model_dir + "mat_demo.obj");
    robot_node->set_material(robot_mat);
    robot_node->set_scale(0.3f);
    my_world.append_node(robot_node);
    node *monkey_node = new node(model_dir + "NormalExample.obj");
    monkey_node->set_material(monkey_mat);
    monkey_node->place(2, 0, 0);
    my_world.append_node(monkey_node);
    node *rtt_node = new node(model_dir + "room.obj");
    rtt_node->set_material(rtt_mat);
    rtt_node->place(7, 2, 0);
    my_world.append_node(rtt_node);
    node *mirror_node = new node(model_dir + "sphere.obj");
    mirror_node->set_material(mirror_mat);
    mirror_node->place(mirror_position.x, mirror_position.y, mirror_position.z);
    my_world.append_node(mirror_node);
    node *room_node = new node(model_dir + "room.obj");
    room_node->set_material(room_mat);
    room_node->set_scale(0.7f);
    room_node->place(-3, 0, 1);
    my_world.append_node(room_node);

    //Unused zoom function
    float zoom = 2.0f;
    my_world.get_parent_node()->set_scale(zoom);

    //Setup main camera and lights
    //camera the_camera(glm::vec3(0, 10, 5), glm::vec3(0, 1, 0));
    camera the_camera(glm::vec3(0, 20, -20), glm::vec3(0, 0, 20));
    the_camera.set_parent(robot_node);
    std::vector<directional_light> directional_lights = {
            directional_light(glm::vec3(100, 100, 100), 0.5, glm::vec3(-2, 5, 2)) };
    std::vector<point_light> point_lights = { point_light(glm::vec3(1.0, 0, 0), 3.0, glm::vec3(0, 0.5, 1.5)),
                                              point_light(glm::vec3(0, 1.0, 0), 3.0, glm::vec3(0, -0.5, -1.5)),
                                              point_light(glm::vec3(0, 0, 1.0), 3.0, glm::vec3(1.0, 0, 0.5)),
    };
    glm::vec3 ambient_light_color = glm::vec3(0.5);

    //Setup rotation and location
    float posx, posy, posz, rot = 0.0f;
    float intensity = 0.0f;

    bool was_key_p_pressed = false;
    bool use_parallax = true;

    //loop
    bool quit = false;
    while(!quit && !win.key_pressed(GLFW_KEY_ESCAPE) && !win.should_close()) {

        //count framerate
        if(glfwGetTime() - fpsc >= 1.0f) {
            std::clog << "-FPS: " << fps << "   \r";

            fps = 0;
            fpsc = glfwGetTime();
        } else {
            fps += 1;
        }

        //Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        offscreen_framebuffer.bind();
        glViewport(0, 0, 128, 128);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //move camera
        if(win.key_pressed(GLFW_KEY_A)) { posx += 0.2; }
        if(win.key_pressed(GLFW_KEY_D)) { posx -= 0.2; }
        if(win.key_pressed(GLFW_KEY_W)) { posz += 0.2; }
        if(win.key_pressed(GLFW_KEY_S)) { posz -= 0.2; }
        if(win.key_pressed(GLFW_KEY_Q)) { rot += 0.1; }
        if(win.key_pressed(GLFW_KEY_E)) { rot -= 0.1; }
        if(win.key_pressed(GLFW_KEY_LEFT_SHIFT)) { posy += 0.1; }
        if(win.key_pressed(GLFW_KEY_LEFT_CONTROL)) { posy -= 0.1; }
        robot_node->rotate(0, rot, 0);
        robot_node->move_relative(posx, posy, posz);
        posx = posy = posz = rot = 0;

        //change light intensity
        intensity += 0.01;
        directional_lights[0].set_intensity(0.5 * sin(intensity) + 0.5);

        if(win.key_pressed(GLFW_KEY_P)) {
            was_key_p_pressed = true;
        } else if(was_key_p_pressed) {
            was_key_p_pressed = false;
            use_parallax = !use_parallax;
        }

        render_pass.render(my_world, the_camera, directional_lights, point_lights, glm::vec3(1.0, 1.0, 1.0),
                           offscreen_framebuffer);
        solid_render_pass.render(my_world, the_camera, directional_lights, point_lights);
        normal_render_pass.render(my_world, the_camera, directional_lights, point_lights, ambient_light_color,
                                  offscreen_framebuffer);

        for(int i = 0; i < 6; ++i) {
            auto framebuff = cubemap_framebuffers[i];
            framebuff->bind();
            glViewport(0, 0, 128, 128);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            auto cam = cubemap_cameras[i];

            render_pass.render(my_world, cam, directional_lights, point_lights, glm::vec3(1.0, 1.0, 1.0),
                               *framebuff);
            solid_render_pass.render(my_world, cam, directional_lights, point_lights);
            normal_render_pass.render(my_world, cam, directional_lights, point_lights, ambient_light_color,
                                      *framebuff);
        }

        screen->bind();
        glViewport(0, 0, 1280, 640);

        render_pass.render(my_world, the_camera, directional_lights, point_lights, ambient_light_color, *screen);
        solid_render_pass.render(my_world, the_camera, directional_lights, point_lights);
        normal_render_pass.render(my_world, the_camera, directional_lights, point_lights, ambient_light_color, *screen);
        rtt_pass.render(my_world, the_camera, directional_lights, point_lights, glm::vec3(1.0, 1.0, 1.0),
                        *screen);
        mirror_pass.render(my_world, the_camera, directional_lights, point_lights, glm::vec3(1.0, 1.0, 1.0),
                                  *screen);
                                  
        if(use_parallax) {
            parallax_render_pass.render(my_world, the_camera, directional_lights, point_lights, ambient_light_color,
                                        *screen);
        } else {
            no_parallax_render_pass.render(my_world, the_camera, directional_lights, point_lights, ambient_light_color,
                                           *screen);
        }
        
        //Update window and events
        win.update();

        //check for OpenGL errors
        try {
            check_gl_error();
        } catch(gl_exception &e) {
            std::cerr << "! Detected GL Error: " << e.what() << '\n';
            quit = true;
        }

    }

}
