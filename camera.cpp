#include "camera.h"

camera::camera() {
    position = glm::vec3(0.0f);
    look_at = glm::vec3(0.0f, 0.0f, -1.0f);
    view = glm::lookAt(position, look_at, glm::vec3(0, 1, 0));
    projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    first_person = false;
}

camera::~camera() {

}

void camera::place(float arg_x, float arg_y, float arg_z) {
    position = glm::vec3(arg_x, arg_y, arg_z);
    view = glm::lookAt(position, look_at, glm::vec3(0, 1, 0));
}

void camera::place(glm::vec3 arg) {
    position = arg;
    view = glm::lookAt(position, look_at, glm::vec3(0, 1, 0));
}

void camera::point_at(float arg_x, float arg_y, float arg_z) {
    look_at = glm::vec3(arg_x, arg_y, arg_z);
    view = glm::lookAt(position, look_at, glm::vec3(0, 1, 0));
}

void camera::point_at(glm::vec3 arg) {
    look_at = arg;
    view = glm::lookAt(position, look_at, glm::vec3(0, 1, 0));
}

void camera::setup(glm::vec3 arg_pos, glm::vec3 arg_point) {
    position = arg_pos;
    look_at = arg_point;
    view = glm::lookAt(position, look_at, glm::vec3(0, 1, 0));
}

void camera::set_first_person(bool arg) {
    first_person = arg;
}

void camera::update_motion() {
    if(first_person) {

    }
}

void camera::update() {
    GLint shader_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &shader_id);
    GLuint view_id = glGetUniformLocation(shader_id, "view");
    glUniformMatrix4fv(view_id, 1, GL_FALSE, &view[0][0]);
    GLuint projection_id = glGetUniformLocation(shader_id, "projection");
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, &projection[0][0]);
}