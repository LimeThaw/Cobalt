#include "window.h"

window::window(int width, int height, std::string title) : width(width), height(height), title(title), samples(4) {

	std::clog << "- Creating new window...";

    //init GLFW
    if(!glfwInit()) {
        std::cerr << " ! Window could not be created: Failed to initialize GLFW\n";
        return;
    } else {
        std::clog << " - Initialized GLFW\n";
    }
    
	//Give GLFW some info
	glfwWindowHint(GLFW_SAMPLES, samples);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	
    //init window
    glfw_window = glfwCreateWindow(width, height, title.c_str(), NULL, 0);
    if(glfw_window == NULL) {
        std::cerr << " ! Failed to open window\n";
        return;
    } else {
        std::cout << " - Opened window\n";
    }
    
    //Setup key capturing
    glfwSetInputMode(glfw_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(glfw_window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
    
    //Make window current
    make_current();
	
    //init GLEW
    if(glewInit() != GLEW_OK) {
        std::cerr << " ! Window could not be created: Failed to initialize GLEW\n";
        return;
    } else {
        std::clog << " - Initialized GLEW\n\n";
    }
    
    //setup some OpneGL functions
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_CUBE_MAP);

}

window::~window() {
	glfwTerminate();
}

void window::make_current() {
    glfwMakeContextCurrent(glfw_window);
}

void  window::update() {
        //Update window and events
        glfwSwapBuffers(glfw_window);
        glfwPollEvents();
}

bool window::key_pressed(int key_code) {
	return glfwGetKey(glfw_window, key_code) == GLFW_PRESS;
}

bool window::should_close() {
	return glfwWindowShouldClose(glfw_window);
}
