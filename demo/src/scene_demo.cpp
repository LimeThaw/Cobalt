#include "cobalt.h"
#include "library/standard.h"

int main() {

	// Create window
	window win(1280, 620, "Cobalt Scene Demo");

    //init framerate counting
    int fps = 0, fpsc = glfwGetTime();

	// Load scene
	cs::std_scene scene;
	scene.load("test.scene");

	// Exit condition
	bool quit = false;

	node_ptr camera_node = scene.find_node("untitled");

	// Main loop
	while(!quit && !win.key_pressed(GLFW_KEY_ESCAPE) && !win.should_close()) {

			if(win.key_pressed(GLFW_KEY_A)) /*camera_node->move(0, .1, 0);*/camera_node->rotate(0, -.05, 0);
			if(win.key_pressed(GLFW_KEY_D)) /*camera_node->move(0, -.1, 0);*/camera_node->rotate(.05, 0, 0);

		// Rendering the scene
		win.clear();
		scene.render();
		win.update();

        // Checking for OpenGL errors (usually shader errors)
        try {
            check_gl_error();
        } catch(gl_exception &e) {
            std::cerr << "! Detected GL Error: " << e.what() << '\n';
            quit = true;
        }

        //count framerate
        if(glfwGetTime() - fpsc >= 1.0f) {
            std::clog << "-FPS: " << fps << "   \r";

            fps = 0;
            fpsc = glfwGetTime();
        } else {
            fps += 1;
        }

	}
}
