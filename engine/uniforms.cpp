#include "uniforms.h"

/**
 *  Getter function. Returns the value that is given to the shader with bind().
 */
template<typename T>
T *material_uniform<T>::get_data() {
	return &data;
}

/**
 *  Getter function. Returns the float value to be given to the shader.
 */
template<>
float* material_uniform<float>::get_data() {
	return &data;
}

/**
 *  Getter function. Returns the 3D vector to be given to the shader.
 */
template<>
glm::vec3* material_uniform<glm::vec3>::get_data() {
	return &data;
}

/**
 *  Setter function. Sets the value that is given to the shader with bind().
 */
template<typename T>
void material_uniform<T>::set_data(T &new_data) {
	data = new_data;
}

/**
 *  Constructor. See set_data().
 */
float_uniform::float_uniform(float new_data) {
	set_data(new_data);
}

/**
 *  Gives the value stored by the object to the active OpenGL shader.
 *  @param	target	The name used to access the value in the shader.
 *  					Add 'uniform float $TARGET;' to the shader, where you replace $TARGET with the
 *  					name given here.
 */
void float_uniform::bind(std::string target) {
    GLint active_shader_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);
    GLuint uniform_id = glGetUniformLocation(active_shader_id, target.c_str());
    glUniform1f(uniform_id, data);
}

/**
 *  Constructor. See set_data().
 */
vec3_uniform::vec3_uniform(glm::vec3 new_vec) {
	set_data(new_vec);
}

/**
 *  Gives the value stored by the object to the active OpenGL shader.
 *  @param	target	The name used to access the value in the shader.
 *  					Add 'uniform vec3 $TARGET;' to the shader, where you replace $TARGET with the
 *  					name given here.
 */
void vec3_uniform::bind(std::string target) {
    GLint active_shader_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);
    GLuint uniform_id = glGetUniformLocation(active_shader_id, target.c_str());
    glUniform3f(uniform_id, data.x, data.y, data.z);

}

/**
 *  Constructor. See set_data().
 */
bool_uniform::bool_uniform(bool new_bool) {
	set_data(new_bool);
}

/**
 *  Gives the value stored by the object to the active OpenGL shader.
 *  @param	target	The name used to access the value in the shader.
 *  					Add 'uniform bool $TARGET;' to the shader, where you replace $TARGET with the
 *  					name given here.
 */
void bool_uniform::bind(std::string target) {
    GLint active_shader_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);
    GLuint uniform_id = glGetUniformLocation(active_shader_id, target.c_str());
    if(data) {
		glUniform1i(uniform_id, 1);
	} else {
		glUniform1i(uniform_id, 0);
	}
}
