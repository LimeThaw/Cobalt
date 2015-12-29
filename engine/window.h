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
		bool should_close();
		unsigned int get_fps();
		void set_fps(unsigned int new_fps);

	private:
		int width, height, samples;
		std::string title;
		unsigned int fps;
		float last_frame_time;
		GLFWwindow *glfw_window;
};

#endif
