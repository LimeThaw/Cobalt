#include "camera.h"

camera::camera(glm::vec3 position, glm::vec3 look_at, glm::mat4 projection) : position(position), look_at(look_at), projection(projection) {
    view = glm::lookAt(position, look_at, glm::vec3(0, 1, 0));
    parent = nullptr;
}

camera::camera(glm::mat4 view, glm::mat4 projection) : view(view), projection(projection) {
    parent = nullptr;
}

camera::~camera() {

}

void camera::set_position(float arg_x, float arg_y, float arg_z) {
    position = glm::vec3(arg_x, arg_y, arg_z);
    view = glm::lookAt(position, look_at, glm::vec3(0, 1, 0));
}

void camera::set_position(glm::vec3 arg) {
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

const glm::mat4 camera::get_view() const {
	if(parent != nullptr) {
		return view * glm::inverse(parent->get_node_matrix());
	} else {
    	return view;
	}
}

const glm::mat4 &camera::get_projection() const {
    return projection;
}

void camera::set_parent(node *new_parent) {
	parent = new_parent;
}
