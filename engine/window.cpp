#include "window.h"

window::window(int width, int height, std::string title) : width(width), height(height), samples(4), title(title) {

	std::clog << indent::get() << "- Creating new window...\n";
	indent::increase();

    //init GLFW
    if(!glfwInit()) {
        std::cerr << indent::get() << "! Window could not be created: Failed to initialize GLFW\n";
		indent::decrease();
        return;
    } else {
        if(WINDOW_INFO)std::clog << indent::get() << "- Initialized GLFW\n";
    }

	//Give GLFW some info
	glfwWindowHint(GLFW_SAMPLES, samples);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //init window
    glfw_window = glfwCreateWindow(width, height, title.c_str(), NULL, 0);
    if(glfw_window == NULL) {
        std::cerr << indent::get() << "! Failed to open window\n";
		indent::decrease();
        return;
    } else {
        std::cout << indent::get() << "- Opened window\n";
    }

    //Setup key capturing
    glfwSetInputMode(glfw_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(glfw_window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

    //Make window current
    make_current();

    //init GLEW
	glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        std::cerr << indent::get() << "! Window could not be created: Failed to initialize GLEW\n";
		indent::decrease();
        return;
    } else {
        if(WINDOW_INFO)std::clog << indent::get() << "- Initialized GLEW\n";
    }
    std::clog << "\n";

    //setup some OpneGL functions
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_CUBE_MAP);

	//setup framerate limiting
	fps = 0; //0 fps -> unlimited
	last_frame_time = glfwGetTime();
	indent::decrease();
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
