/**
The window class is used to initialize an OpenGL context and open a window
*/

#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

class window {
	public:
		window(int width, int height, std::string title);
		~window();
		void make_current();
		void update();
		bool key_pressed(int key_code);

	private:
		int width, height, samples;
		std::string title;
		GLFWwindow *glfw_window;
};

#endif
