#ifndef SHADER_UNIFORM_H
#define SHADER_UNIFORM_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>

class uniform {

	public:
		typedef std::shared_ptr<uniform> ptr;
		virtual void bind(std::string target) = 0;

};

template<typename T>
class material_uniform : public uniform {

	public:
		T *get_data();
		void set_data(T &new_data);
		virtual void bind(std::string target) = 0;
	protected:
		T data;

};

class float_uniform : public material_uniform<float> {

	public:
		float_uniform(float new_data);
		void bind(std::string target);

};

class vec3_uniform : public material_uniform<glm::vec3> {
	public:
		vec3_uniform(glm::vec3 new_vec);
		void bind(std::string target);
};

#endif
