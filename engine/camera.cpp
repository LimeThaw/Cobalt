#include "camera.h"

/**
 *  This constructor sets the position of the camera, the direction in looks and its projection matrix.
 *  It also calls the node() constructor on itself.
 *  @param	position	The position of the camera in 3D space.
 *  @param	look_point	A point in 3D space. The orientation of the camera will be towards
 *  						this point.
 *  @param	projection	The projection matrix used by the camera for rendering.
 *  						Can be optained using glm::perspective.
 *
 *  All parameters have default values and as such can be left blank, which makes camera() a perfectly
 *  valid call to this function.
 */
camera::camera(glm::vec3 position, glm::vec3 look_point, glm::mat4 projection) : projection(projection) {
	node();
	place(position);
    look_at(look_point);
}

/**
 *  A constructor which takes two matrices that describe the state of the camera
 *  @param	view		The matrix that describes position, scale and orientation of the camera
 *  						node.
 *  @param	projection The matrix describing the projection of the camera, including field of view,
 *  						clip planes etc.
 *
 *  See [here](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/ "OpenGL Tutorial") for further
 *  information on view and projection matrices
 */
camera::camera(glm::mat4 view, glm::mat4 projection) : projection(projection) {
    parent_node = nullptr;
    node_matrix = view;
}

/** The destructor of the camera class. Essentially does nothing. */
camera::~camera() {

}

/**
 *  Similar to the three-argument-constructor, but without projection matrix.
 *  Places the camera at arg_pos and makes it look in the direction of arg_point.
 *  @param	position	The position of the camera in 3D space.
 *  @param	look_point	A point in 3D space. The orientation of the camera will be towards
 *  						this point.
 */
void camera::setup(glm::vec3 arg_pos, glm::vec3 arg_point) {
    place(arg_pos);
    look_at(arg_point);
}

/**
 *  Returns the view matrix of the camera. It will consider the global position of the camera, meaning
 *  that any transformations of parent nodes will be taken into account.
 */
const glm::mat4 camera::get_view() const {
	if(parent_node != nullptr) {
		return glm::inverse(parent_node->get_node_matrix() * node_matrix);
	} else {
    	return glm::inverse(node_matrix);
	}
}

/** Returns the projection matrix. No tricks here, just a plain old get. */
const glm::mat4 &camera::get_projection() const {
    return projection;
}

/** Sets the projection matrix to the given value. See
 *  [here](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/ "OpenGL Tutorial") for further
 *  information on view and projection matrices.
 */
void camera::set_projection(const glm::mat4 &matrix) {
	projection = matrix;
}
