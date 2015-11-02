#include "camera.h"

camera::camera(glm::vec3 position, glm::vec3 look_at, glm::mat4 projection) : projection(projection) {
    node_matrix = glm::lookAt(position, look_at, glm::vec3(0, 1, 0));
    parent_node = nullptr;
}

camera::camera(glm::mat4 view, glm::mat4 projection) : projection(projection) {
    parent_node = nullptr;
    node_matrix = view;
}

camera::~camera() {

}

void camera::setup(glm::vec3 arg_pos, glm::vec3 arg_point) {
    place(arg_pos);
    look_at(arg_point);
}

const glm::mat4 camera::get_view() const {
	if(parent_node != nullptr) {
		return node_matrix * glm::inverse(parent_node->get_node_matrix());
	} else {
    	return node_matrix;
	}
}

const glm::mat4 &camera::get_projection() const {
    return projection;
}
