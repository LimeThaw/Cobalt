#ifndef SHADER_UNIFORM_H
#define SHADER_UNIFORM_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>

/**
 *  Abstract class used as template for uniforms. Uniforms are values that will be used
 *  in a shader, and thus have to be handled by OpenGL. The virtual bind() method makes
 *  sure every uniform implements a method handling the handover.
 */
class uniform {

	public:
		typedef std::shared_ptr<uniform> ptr; ///< Convenience alias
		virtual void bind(std::string target) = 0; ///< In the implementation, this should make sure the value makes it to the shader.

};

/**
 *  Uniform to be used with a Cobalt material. Adds the data attribute and some access
 *  functions to the uniform class to make it compatible to materials.
 */
template<typename T>
class material_uniform : public uniform {

	public:
		T *get_data();
		void set_data(T &new_data);
		virtual void bind(std::string target) = 0; ///< In the implementation, this should make sure the value makes it to the shader.
	protected:
		T data; ///< The data to be used in the shader

};

/**
 *  Specialization of the material_uniform class for float values.
 */
class float_uniform : public material_uniform<float> {

	public:
		float_uniform(float new_data);
		void bind(std::string target);

};

/**
 *  Specialization of the material_uniform class for 3D vectors.
 */
class vec3_uniform : public material_uniform<glm::vec3> {
	public:
		vec3_uniform(glm::vec3 new_vec);
		void bind(std::string target);
};

/**
 *  Specialization of the material_uniform class for boolean values.
 */
class bool_uniform : public material_uniform<bool> {
	public:
		bool_uniform(bool new_bool);
		void bind(std::string target);
};

#endif
