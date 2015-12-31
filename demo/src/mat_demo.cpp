#include "cobalt.h"
#include "library/standard.h"

int main() {

	// Creating window object
	window win(1280, 620, "Cobalt Material Demo");
	win.set_fps(60);
	
	//Creating scene and light(s)
    cs::std_scene demo_world;
	demo_world.add_directional_light(directional_light(glm::vec3(100, 100, 100), 0.5, glm::vec3(-5, 5, -10)));
    demo_world.add_point_light(point_light(glm::vec3(0.3, 0.3, 1.0), 3.0f, glm::vec3(3, 3, -5)));
    
    // Loading the red sphere
	auto red_mat = std::make_shared<cs::std_material>(glm::vec3(2.0f, 0.5f, 0.5f), 0.5f, 1.0f);
	mesh *red_mesh = new mesh("./demo/res/models/mat_demo.obj");
    red_mesh->set_material(red_mat);
    red_mesh->place(-5, 0, 0);
    demo_world.append_mesh(red_mesh);
    
    // Loading the green sphere
	auto green_mat = std::make_shared<cs::std_material>(glm::vec3(0.5f, 1.0f, 0.5f), 0.5f, 1.0f);
	mesh *green_mesh = new mesh("./demo/res/models/mat_demo.obj");
    green_mesh->set_material(green_mat);
    demo_world.append_mesh(green_mesh);
    
    // Loading the blue sphere
	auto blue_mat = std::make_shared<cs::std_material>(glm::vec3(0.5f, 0.5f, 2.0f), 0.5f, 1.0f);
	mesh *blue_mesh = new mesh("./demo/res/models/mat_demo.obj");
    blue_mesh->set_material(blue_mat);
    blue_mesh->place(5, 0, 0);
    demo_world.append_mesh(blue_mesh);

	// Loading chair
	auto chair_mat = std::make_shared<cs::std_material>();
	chair_mat->set_color_map("./demo/res/textures/stuhl_color.png");
	chair_mat->set_bump_map("./demo/res/textures/stuhl_height.png");
	chair_mat->set_shader_mask("./demo/res/textures/stuhl_spec.png");
	mesh *chair_mesh = new mesh("./demo/res/models/stuhl.obj");
	chair_mesh->set_material(chair_mat);
	chair_mesh->place(0, 0, -5);
	demo_world.append_node(chair_mesh);
	
	// Creating the camera
	auto main_camera = std::make_shared<camera>(glm::vec3(0, 5, -8), glm::vec3(0, 0, 0));
	demo_world.set_camera(main_camera);
	
	// Introducing the roughness variable
	// Used to dynamically control the roughness of the spheres
	float roughness = 0.5f;
	
	// Declaring variables used for framerate counting
	float fpsc = glfwGetTime();
	int fps = 0;
	
	// Main loop
	bool quit = false;
	while(!quit && !win.key_pressed(GLFW_KEY_ESCAPE) && !win.should_close()) {

		// Introducing angle variable for turning the chair
		float angle = 0.0f;
	
		// Checking for key strokes
		if(win.key_pressed(GLFW_KEY_W)) {
			if(roughness < 1.0f) roughness += 0.01f;
		}
		if(win.key_pressed(GLFW_KEY_S)) {
			if(roughness > 0.0f) roughness -= 0.01f;
		}
		if(win.key_pressed(GLFW_KEY_Q)) {
			angle -= 0.02;
		}
		if(win.key_pressed(GLFW_KEY_E)) {
			angle += 0.02;
		}

		// Rotating the chair
		chair_mesh->rotate(0, angle, 0);
		
		// Setting roughness for the meshes
		red_mat->set_roughness(roughness);
		green_mat->set_roughness(roughness);
		blue_mat->set_roughness(roughness);
		
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

