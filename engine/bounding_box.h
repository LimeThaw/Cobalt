#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

/**
 *  Simple data struct representing a box in 3D space by storing all of its corner vertices.
 *  The struct obviously supports axis-aligned and non-aligned bounding boxes.
 */
struct box {
	glm::vec3 vertices[8]; ///< The corner vertices of the box.
};

/**
 *  A simple bounding box. It is stored using a position vector and three edge vectors, which
 *  makes it a Parallelepiped (https://en.wikipedia.org/wiki/Parallelepiped). Implemented for
 *  use in frustum computation for shadow map generation, might have some use for physics
 *  application later...
 *  \image html box.png
 */
class bounding_box {
	public:
		bounding_box();
		bounding_box(glm::vec3 min, glm::vec3 max);
		bounding_box(const box &arg_box);
		bounding_box(glm::vec3 direction);
		box get_vertices() const;
		void add_vertex(glm::vec3 vertex);
		void add_bounding_box(const bounding_box &arg_box);
		glm::mat4 get_light_matrix();

	private:
		glm::vec3 position; ///< The position of the box. Or it's first vertex.
		glm::vec3 sides[3]; ///< Three edges of the box. Or 3 of its vertices relative to the first. The other four can be calculated from the fist four.
};

bounding_box operator * (glm::mat4 matrix, const bounding_box &arg_box);

#endif
