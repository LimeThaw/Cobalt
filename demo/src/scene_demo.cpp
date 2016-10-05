#include "library/standard.h"

int main() {

	// Create window
	window win(1280, 620, "Cobalt Scene Demo");
	
	// Load scene
	cs::std_scene scene;
	scene.load("test.scene");
	
	// Exit condition
	bool quit = false;
	
	// Main loop
	while(!quit && !win.key_pressed(GLFW_KEY_ESCAPE) && !win.should_close()) {
	
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
