#include "window.h"

window::window(int width, int height, std::string title) : width(width), height(height), title(title), samples(4) {

	std::clog << "- Creating new window...\n";

    //init GLFW
    if(!glfwInit()) {
        std::cerr << " ! Window could not be created: Failed to initialize GLFW\n";
        return;
    } else {
        if(WINDOW_INFO)std::clog << " - Initialized GLFW\n";
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
        if(WINDOW_INFO)std::clog << " - Initialized GLEW\n";
    }
    std::clog << "\n";
    
    //setup some OpneGL functions
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_CUBE_MAP);

	//setup framerate limiting
	fps = 0; //0 fps -> unlimited
	last_frame_time = glfwGetTime();
}

window::~window() {
	glfwTerminate();
}

void window::make_current() {
    glfwMakeContextCurrent(glfw_window);
}

void  window::update() {
		//Limit framerate
		if(fps > 0) {
			while((float)(glfwGetTime() - last_frame_time) < (float)(1.0f / fps)){}
			last_frame_time = glfwGetTime();
		}
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

unsigned int window::get_fps() {
	return fps;
}
void window::set_fps(unsigned int new_fps) {
	fps = new_fps;
}

void window::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
