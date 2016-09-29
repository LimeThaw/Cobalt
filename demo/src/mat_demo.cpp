#include "cobalt.h"
#include "library/standard.h"

int main() {

	// Creating window object
	window win(1280, 620, "Cobalt Lighting Demo");
	win.set_fps(60);
	
	//Creating scene and light(s)
    cs::std_scene demo_world;
    demo_world.set_skybox("demo/res/textures/skybox.png");
    auto light = std::make_shared<directional_light>(glm::vec3(100, 100, 100), 0.5f, glm::vec3(0, 1, 1), "rotating_light");
	//demo_world.add_directional_light(std::make_shared<directional_light>(glm::vec3(100, 100, 100), 0.0f, glm::vec3(1, 1, 1)));
	demo_world.add_directional_light(light);
    demo_world.add_point_light(std::make_shared<point_light>(glm::vec3(0.3, 0.3, 1.0), 3.0f, glm::vec3(0, 1.8, 0), 10.0f, "inner_light"));
	
	// Loading sample meshes
	auto sample_mat = std::make_shared<cs::std_material>("sample_mat");
	mesh* sample_mesh = new mesh("demo/res/models/untitled.obj", "untitled");
	sample_mesh->set_scale(0.3f);
	sample_mesh->set_material(sample_mat);
	demo_world.append_node(sample_mesh);
	
	auto terrain_mat = std::make_shared<cs::std_material>("terrain_mat");
	terrain_mat->set_color_map(texture2d::load_file("demo/res/textures/testmapTex_small.png", "terrain_tex"));
	mesh* terrain_mesh = new mesh("demo/res/models/testmap.obj", "terrain_mesh");
	terrain_mesh->set_material(terrain_mat);
	demo_world.append_node(terrain_mesh);
	
	sample_mat = std::make_shared<cs::std_material>("sphere_mat");
	sample_mat->set_shadow_cast(false);
	mesh* test_mesh = new mesh("demo/test_sphere.obj", "sphere");
	test_mesh->set_scale(0.3);
	test_mesh->place(0, 1.8, 0);
	test_mesh->set_material(sample_mat);
	demo_world.append_node(test_mesh);
	
	// Creating the camera
	auto main_camera = std::make_shared<camera>(glm::vec3(0, 10, -8), glm::vec3(0, 1.5, 0));
	main_camera->set_name("main_camera");
	demo_world.set_camera(main_camera);
	
	// Declaring variables used for framerate counting
	float fpsc = glfwGetTime();
	int fps = 0;
	
	// Write scene to file "test.scene"
	demo_world.save("test.scene");
	
	// Main loop
	bool quit = false;
	float rotation = 0.0f, light_angle = 0.0f, angle = 0.0f;
	while(!quit && !win.key_pressed(GLFW_KEY_ESCAPE) && !win.should_close()) {

		// Introducing angle and luminosity variables for turning the helmet
		//float angle = 0.0f;
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
			rotation -= 0.01f;
		}
		if(win.key_pressed(GLFW_KEY_D)) {
			rotation += 0.01f;
		}

		// Rotating the model
		//sample_mesh->rotate(0, angle, 0);
		//sample_mat->set_luminosity(luminosity);
		
		// Rotate the camera
		main_camera->setup(glm::vec3(8 * (1 + rotation) * sin(angle), 10 * (1 + rotation), -8 * (1 + rotation) * cos(angle)), glm::vec3(0, 1.5 * (1 + rotation), 0));
		//sample_mesh->set_scale(0.3f + 0.1f * rotation);
		
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

