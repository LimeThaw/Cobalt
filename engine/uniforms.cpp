#include "uniforms.h"

GLuint uniform::get_uniform_location(const std::string &target) {
    GLint active_shader_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);
    GLuint uniform_id = glGetUniformLocation(active_shader_id, target.c_str());
    return uniform_id;
}

template<typename T>
T *material_uniform<T>::get_data() {
	return &data;
}

template<typename T>
void material_uniform<T>::set_data(const T &new_data) {
	data = new_data;
}

float_uniform::float_uniform(float new_data) {
	set_data(new_data);
}

void float_uniform::bind(std::string target) {
    GLuint uniform_id = get_uniform_location(target);
    glUniform1f(uniform_id, data);
}

vec3_uniform::vec3_uniform(glm::vec3 new_vec) {
	set_data(new_vec);
}

void vec3_uniform::bind(std::string target) {
    GLuint uniform_id = get_uniform_location(target);
    glUniform3f(uniform_id, data.x, data.y, data.z);
}

mat4_uniform::mat4_uniform(const glm::mat4 &new_mat) {
  set_data(new_mat);
}

void mat4_uniform::bind(std::string target) {
  GLuint uniform_id = get_uniform_location(target);
  glUniformMatrix4fv(uniform_id, 1, GL_FALSE, &data[0][0]);
}

array_uniform::array_uniform(const std::vector<std::shared_ptr<uniform>> &data) : data(data) {
}

void array_uniform::bind(std::string target) {
  for(unsigned int i = 0; i < data.size(); ++i) {
    std::string this_target = target + "[" + std::to_string(i) + "]";
    data[i]->bind(this_target);
  }
}
    