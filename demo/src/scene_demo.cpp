//FIXME: Fix point light shadows being only on one side and not rotating properly with the parent node

#include "cobalt.h"
#include "library/standard.h"

int main() {

	// Create window
	window win(1280, 620, "Cobalt Scene Demo");

	// Load scene
	cs::std_scene scene;
	scene.load("test.scene");

	// Exit condition
	bool quit = false;

	node_ptr camera_node = scene.find_node("camera_node");

	// Main loop
	while(!quit && !win.key_pressed(GLFW_KEY_ESCAPE) && !win.should_close()) {

			if(win.key_pressed(GLFW_KEY_A)) camera_node->rotate(0, -.05, 0);
			if(win.key_pressed(GLFW_KEY_D)) camera_node->rotate(0, .05, 0);

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

	}
}
