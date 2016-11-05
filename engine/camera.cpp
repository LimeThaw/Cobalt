#include "camera.h"

camera::camera(glm::vec3 position, glm::vec3 look_point, glm::mat4 projection) : projection(projection) {
	node();
	place(position);
    look_at(look_point);
    parent_node = weak_ptr<node>();
}

camera::camera(glm::mat4 view, glm::mat4 projection) : projection(projection) {
    parent_node = weak_ptr<node>();
    node_matrix = view;
}

camera::~camera() {

}

void camera::setup(glm::vec3 arg_pos, glm::vec3 arg_point) {
    place(arg_pos);
    look_at(arg_point);
}

const glm::mat4 camera::get_view() const {
	shared_ptr<node> pn;
	if(!parent_node.expired() && (pn = parent_node.lock()) != nullptr) {
		return glm::inverse(pn->get_node_matrix() * node_matrix);
	} else {
    	return glm::inverse(node_matrix);
	}
}

const glm::mat4 &camera::get_projection() const {
    return projection;
}

void camera::set_projection(const glm::mat4 &matrix) {
	projection = matrix;
}
