#ifndef SHADER_UNIFORM_H
#define SHADER_UNIFORM_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class uniform {

	public:
		virtual void bind(std::string target) = 0;

};

template<typename T>
class shader_uniform : public uniform {

	public:
		T *get_data();
		void set_data(T &new_data);
		virtual void bind(std::string target) = 0;
	protected:
		T data;

};

class float_uniform : public shader_uniform<float> {

	public:
		float_uniform(float new_data);
		void bind(std::string target);

};

#endif
