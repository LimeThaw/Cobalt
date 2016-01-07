#include "cobalt.h"
#include "library/standard.h"

int main() {

	// Creating window object
	window win(1280, 620, "Cobalt Material Demo");
	win.set_fps(60);
	
	//Creating scene and light(s)
    cs::std_scene demo_world;
    demo_world.set_skybox("demo/res/textures/skybox.png");
	demo_world.add_directional_light(directional_light(glm::vec3(100, 100, 100), 0.2, glm::vec3(-5, 5, -10)));
    demo_world.add_point_light(point_light(glm::vec3(0.3, 0.3, 1.0), 3.0f, glm::vec3(3, 3, -5)));
	
	// Loading helmet
	auto helm_mat = std::make_shared<cs::std_material>();
	helm_mat->set_color_map("demo/res/textures/helm_colormap.png");
	helm_mat->set_normal_map("demo/res/textures/NormalMap.png");
	helm_mat->set_shader_mask("demo/res/textures/helm_bumpmap.png");
	mesh* helm_mesh = new mesh("demo/res/models/helm.obj");
	helm_mesh->set_scale(2.0f);
	helm_mesh->set_material(helm_mat);
	demo_world.append_node(helm_mesh);
	
	// Creating the camera
	auto main_camera = std::make_shared<camera>(glm::vec3(0, 5, -8), glm::vec3(0, 1.5, 0));
	demo_world.set_camera(main_camera);
	
	// Declaring variables used for framerate counting
	float fpsc = glfwGetTime();
	int fps = 0;
	
	// Main loop
	bool quit = false;
	while(!quit && !win.key_pressed(GLFW_KEY_ESCAPE) && !win.should_close()) {

		// Introducing angle variable for turning the helmet
		float angle = 0.0f;
	
		// Checking for key strokes
		if(win.key_pressed(GLFW_KEY_Q)) {
			angle -= 0.02;
		}
		if(win.key_pressed(GLFW_KEY_E)) {
			angle += 0.02;
		}

		// Rotating the helmet
		helm_mesh->rotate(0, angle, 0);
		
		// Rendering the scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		demo_world.render();
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

