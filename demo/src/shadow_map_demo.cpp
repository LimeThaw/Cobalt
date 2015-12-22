//Sample program for Project Cobalt

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <ctime>

#include "cobalt.h"
#include "library/standard.h"
#include "simple_render_pass.h"
#include "depth_only_render_pass.h"

int main() {

    window win(1280, 640, "Cobalt Demo");

    const std::string model_dir = "./demo/res/models/";
    const std::string shader_dir = "./demo/res/shaders/";
    const std::string texture_dir = "./demo/res/textures/";


    //Load shaders and textures
    auto textured_shader = std::make_shared<shader>(shader_dir + "shadowMappingVertexShader.glsl",
                                                    shader_dir + "shadowMappingFragmentShader.glsl");

    auto shadow_map = std::make_shared<texture2d>(512, 512, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT,
						  GL_CLAMP, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    
    auto map_tex = std::make_shared<texture2d>(
            texture_cache::get_instance().get_texture_from_filename(texture_dir + "testmapTex_small.png"));
    
    auto map_mat = std::make_shared<material>(
            material::texture_bindings { 
	      material::texture_binding("color_map", map_tex),
	      material::texture_binding("shadow_map", shadow_map)
	  });

    auto rockwall_tex = std::make_shared<texture2d>(
            texture_cache::get_instance().get_texture_from_filename(texture_dir + "rockwall.jpg"));
   
    auto room_mat = std::make_shared<material>(
	  material::texture_bindings {
		  material::texture_binding("color_map", rockwall_tex),
		  material::texture_binding("shadow_map", shadow_map)
            });

    simple_render_pass render_pass(textured_shader, std::vector<std::shared_ptr<material>> { map_mat, room_mat });
    
    auto depth_only_shader = std::make_shared<shader>(shader_dir + "depthOnlyVertexShader.glsl",
						      shader_dir + "depthOnlyFragmentShader.glsl");
    depth_only_render_pass shadow_pass(depth_only_shader);
    
    scene my_world;
    mesh *map_node = new mesh(model_dir + "testmap.obj");
    map_node->set_material(map_mat);
    my_world.append_mesh(map_node);
    
    std::srand(std::time(0));
    for(int i = 0; i < 30; ++i) {
      node *room_node = new node(model_dir + "room.obj");
      room_node->set_material(room_mat);
      room_node->set_scale(1.0f * (std::rand() / (float) RAND_MAX));
      room_node->place(-10 + 20.0f * (std::rand() / (float) RAND_MAX), 20.0f * (std::rand() / (float) RAND_MAX), -10 + 20.0f * (std::rand() / (float) RAND_MAX));
      my_world.append_node(room_node);
    }

    //Unused zoom function
    float zoom = 2.0f;
    my_world.get_parent_node()->set_scale(zoom);

    camera the_camera(glm::vec3(0, 20, -20), glm::vec3(0, 0, 20));
    std::vector<directional_light> directional_lights = {
            directional_light(glm::vec3(1, 1, 1), 1, glm::vec3(0, 1, 0)) 
    };
    glm::vec3 ambient_light_color = glm::vec3(0.1);
    
    auto screen = framebuffer::get_screen();
    auto shadow_map_attachment = std::make_shared<texture_framebuffer_attachment>(shadow_map);
    auto shadow_map_framebuffer = std::make_shared<framebuffer>(framebuffer::attachments(),
								framebuffer::optional_attachment(shadow_map_attachment));
    
    auto shadow_map_view_matrix = glm::lookAt(glm::vec3(0, 30, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
    auto shadow_map_projection_matrix = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 100.0f);
    auto shadow_map_view_projection_matrix = shadow_map_projection_matrix * shadow_map_view_matrix;
    //also apply bias, because tex coord is [0;1], clip space is [-1;1]
    glm::mat4 bias_matrix(
      0.5, 0.0, 0.0, 0.0,
      0.0, 0.5, 0.0, 0.0,
      0.0, 0.0, 0.5, 0.0,
      0.5, 0.5, 0.5, 1.0
    );
    
    map_mat->set_uniform("shadow_map_view_projection_matrix", std::make_shared<mat4_uniform>(bias_matrix * shadow_map_view_projection_matrix));

    //loop
    bool quit = false;
    while(!quit && !win.key_pressed(GLFW_KEY_ESCAPE) && !win.should_close()) {

        //move camera
        if(win.key_pressed(GLFW_KEY_A)) { the_camera.move_relative(-0.1, 0, 0); }
        if(win.key_pressed(GLFW_KEY_D)) { the_camera.move_relative(0.1, 0, 0); }
        if(win.key_pressed(GLFW_KEY_W)) { the_camera.move_relative(0, 0, -0.1); }
        if(win.key_pressed(GLFW_KEY_S)) { the_camera.move_relative(0, 0, 0.1); }
        if(win.key_pressed(GLFW_KEY_Q)) { the_camera.rotate(0, 0.01, 0); }
        if(win.key_pressed(GLFW_KEY_E)) { the_camera.rotate(0, -0.01, 0); }
        if(win.key_pressed(GLFW_KEY_LEFT_SHIFT)) { the_camera.move_relative(0, 0.1, 0); }
        if(win.key_pressed(GLFW_KEY_LEFT_CONTROL)) { the_camera.move_relative(0, -0.1, 0); }
	
	shadow_map_framebuffer->bind();
	glViewport(0, 0, 512, 512);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	shadow_pass.render(my_world, shadow_map_view_projection_matrix, *shadow_map_framebuffer);
	
	screen->bind();
	glViewport(0, 0, 1280, 640);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
        render_pass.render(my_world, the_camera, directional_lights,
			   std::vector<point_light>(), ambient_light_color, *screen);
        
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
