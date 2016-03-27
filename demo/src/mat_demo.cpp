#include "cobalt.h"
#include "library/standard.h"

int main() {

	// Creating window object
	window win(1280, 620, "Cobalt Material Demo");
	win.set_fps(60);
	
	//Creating scene and light(s)
    cs::std_scene demo_world;
    demo_world.set_skybox("demo/res/textures/skybox.png");
    auto light = std::make_shared<directional_light>(glm::vec3(100, 100, 100), 0.5f, glm::vec3(0, 1, 1));
	demo_world.add_directional_light(std::make_shared<directional_light>(glm::vec3(100, 100, 100), 1.0f, glm::vec3(1, 1, 0)));
	demo_world.add_directional_light(light);
    //demo_world.add_point_light(point_light(glm::vec3(0.3, 0.3, 1.0), 3.0f, glm::vec3(3, 3, -5)));
	
	// Loading helmet
	auto sample_mat = std::make_shared<cs::std_material>();
	mesh* sample_mesh = new mesh("demo/res/models/vase.obj");
	sample_mesh->set_scale(1.0f);
	sample_mesh->set_material(sample_mat);
	demo_world.append_node(sample_mesh);
	
	// Creating the camera
	auto main_camera = std::make_shared<camera>(glm::vec3(0, 10, -8), glm::vec3(0, 1.5, 0));
	demo_world.set_camera(main_camera);
	
	// Declaring variables used for framerate counting
	float fpsc = glfwGetTime();
	int fps = 0;
	
	// Main loop
	bool quit = false;
	float rotation = 0.0f, light_angle = 0.0f;
	while(!quit && !win.key_pressed(GLFW_KEY_ESCAPE) && !win.should_close()) {

		// Introducing angle and luminosity variables for turning the helmet
		float angle = 0.0f;
		float luminosity;
	
		// Checking for key strokes
		if(win.key_pressed(GLFW_KEY_Q)) {
			angle -= 0.02;
		}
		if(win.key_pressed(GLFW_KEY_E)) {
			angle += 0.02;
		}
		if(win.key_pressed(GLFW_KEY_W)) {
			if(luminosity < 1.0f) luminosity += 0.02f;
		}
		if(win.key_pressed(GLFW_KEY_S)) {
			if(luminosity > 0.0f) luminosity -= 0.02f;
		}
		if(win.key_pressed(GLFW_KEY_A)) {
			rotation -= 0.05f;
		}
		if(win.key_pressed(GLFW_KEY_D)) {
			rotation += 0.05f;
		}

		// Rotating the model
		sample_mesh->rotate(0, angle, 0);
		sample_mat->set_luminosity(luminosity);
		
		// Rotate the camera
		main_camera->setup(glm::vec3(8 * sin(rotation), 10, -8 * cos(rotation)), glm::vec3(0, 1.5, 0));
		
		// Rotate the light
		light_angle += 0.01;
		light->set_direction(glm::vec3(sin(light_angle), 1, cos(light_angle)));
		
		// Rendering the scene
		win.clear();
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

