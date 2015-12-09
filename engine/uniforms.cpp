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
