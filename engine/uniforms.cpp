#include "uniforms.h"

template<typename T>
T *material_uniform<T>::get_data() {
	return &data;
}

template<typename T>
void material_uniform<T>::set_data(T &new_data) {
	data = new_data;
}

float_uniform::float_uniform(float new_data) {
	set_data(new_data);
}

void float_uniform::bind(std::string target) {
    GLint active_shader_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);
    GLuint uniform_id = glGetUniformLocation(active_shader_id, target.c_str());
    glUniform1f(uniform_id, data);
}

vec3_uniform::vec3_uniform(glm::vec3 new_vec) {
	set_data(new_vec);
}

void vec3_uniform::bind(std::string target) {
    GLint active_shader_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);
    GLuint uniform_id = glGetUniformLocation(active_shader_id, target.c_str());
    glUniform3f(uniform_id, data.x, data.y, data.z);

}

bool_uniform::bool_uniform(bool new_bool) {
	set_data(new_bool);
}

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
