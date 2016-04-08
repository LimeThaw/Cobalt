#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct box {
	glm::vec3 vertices[8];
};

class bounding_box {
	public:
		bounding_box();
		bounding_box(glm::vec3 min, glm::vec3 max);
		bounding_box(box arg_box);
		bounding_box(glm::vec3 direction);
		box get_vertices();
		void add_vertex(glm::vec3 vertex);
		void add_bounding_box(bounding_box arg_box);
		glm::mat4 get_light_matrix();
	
	private:
		glm::vec3 position;
		glm::vec3 sides[3];
};

bounding_box operator * (glm::mat4 matrix, bounding_box arg_box);

#endif
