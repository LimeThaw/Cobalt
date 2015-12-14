#include "cobalt.h"
#include "library/standard.h"
#include "simple_render_pass.h"

int main() {

	window win(1280, 620, "Cobalt Material Demo");
	
	auto smooth_mat = std::make_shared<cs::std_material>(glm::vec3(1.0f), 0.0f);
	auto medium_mat = std::make_shared<cs::std_material>();
	auto rough_mat = std::make_shared<cs::std_material>(glm::vec3(1.0f), 1.0f);
	
    scene demo_world;
    
	mesh *smooth_mesh = new mesh("./demo/res/models/mat_demo.obj");
    smooth_mesh->set_material(smooth_mat);
    smooth_mesh->place(-5, 0, 0);
    demo_world.append_mesh(smooth_mesh);
    
	mesh *medium_mesh = new mesh("./demo/res/models/mat_demo.obj");
    medium_mesh->set_material(medium_mat);
    demo_world.append_mesh(medium_mesh);
    
	mesh *rough_mesh = new mesh("./demo/res/models/mat_demo.obj");
    rough_mesh->set_material(rough_mat);
    rough_mesh->place(5, 0, 0);
    demo_world.append_mesh(rough_mesh);
	
	std::vector<directional_light> d_lights = {
            directional_light(glm::vec3(100, 100, 100), 0.5, glm::vec3(-2, 5, -10)) };
    std::vector<point_light> p_lights = {};
	
	cs::std_render_pass solid_render_pass;
	camera main_camera(glm::vec3(0, 5, -8), glm::vec3(0, 0, 0));
	
	bool quit = false;
	while(!quit && !win.key_pressed(GLFW_KEY_ESCAPE) && !win.should_close()) {
		
		framebuffer::get_screen()->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		solid_render_pass.render(demo_world, main_camera, d_lights, p_lights);
		win.update();
		
        try {
            check_gl_error();
        } catch(gl_exception &e) {
            std::cerr << "! Detected GL Error: " << e.what() << '\n';
            quit = true;
        }
	
	}

}

