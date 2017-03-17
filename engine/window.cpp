#include "window.h"

/**
 *  The constructor of the window. Calling this creates an OpenGL context, ideally of version 4.30,
 *  initializes helper libraries (GLFW, GLEW), and opens a new window.
 *  @param	width, height	The dimensions of the window that is opened and its canvas.
 *  @param	title			The title of the window. This will be displayedin the title bar.
 */
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

    //setup some OpenGL functions
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_CUBE_MAP);

	//setup framerate limiting
	fps = 0; //0 fps -> unlimited
	last_frame_time = glfwGetTime();
	indent::decrease();
}

/**
 *  Destructor. Makes sure everything is tidied up.
 */
window::~window() {
	glfwTerminate();
}

/**
 *  Makes the window's context current. This means that all drawing and rendering by OpenGL will
 *  reference this context. Use this before rendering if you have more than one window.
 */
void window::make_current() {
    glfwMakeContextCurrent(glfw_window);
}

/**
 *  Updates the window and it's context. It flips the buffers to make the current frame visible and
 *  polls events to ensure the window remains responsive. This should be called in a main loop.
 *  If it is never called the window will not respond to anything.
 */
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

/**
 *  Checks for keypresses on the given key. Use GLFW keycodes
 *  (http://www.glfw.org/docs/latest/group__keys.html).
 */
bool window::key_pressed(int key_code) {
	return glfwGetKey(glfw_window, key_code) == GLFW_PRESS;
}

/**
 *  Checks wether the window was closed. If someone clicked the red X in the corner, then this
 *  returns true. Check it if you have a main loop.
 */
bool window::should_close() {
	return glfwWindowShouldClose(glfw_window);
}

/**
 *  Returns the current FPS limit. If no limit is set it will return 0.
 */
unsigned int window::get_fps() {
	return fps;
}

/**
 *  Sets the FPS limit of the window. Set to 0 for UNLIMITED SPEED!!!!!1elf
 */
void window::set_fps(unsigned int new_fps) {
	fps = new_fps;
}

/**
 *  Clears the window's canvas. Usually good to use before rendering to avoid leftovers, artifacs
 *  and stuff like that.
 */
void window::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
