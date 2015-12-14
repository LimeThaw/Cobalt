#include "camera.h"

camera::camera(glm::vec3 position, glm::vec3 look_point, glm::mat4 projection) : projection(projection) {
	place(position);
    look_at(look_point);
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
		return glm::inverse(parent_node->get_node_matrix() * node_matrix);
	} else {
    	return glm::inverse(node_matrix);
	}
}

const glm::mat4 &camera::get_projection() const {
    return projection;
}
