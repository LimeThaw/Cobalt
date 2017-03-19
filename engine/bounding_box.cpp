#include "bounding_box.h"

/**
 *  Default constructor. Creates a bounding box without volume at the origin. Not very useful,
 *  but initializes the object.
 */
bounding_box::bounding_box() {
	bounding_box(glm::vec3(0.0f), glm::vec3(0.0f));
}

/**
 *  Axis-aligned constructor. Takes two points in space and creates an axis-aligned
 *  bounding box that includes the whole space between them. This is used to calculate
 *  the boundng box of a mesh at load (see mesh::load_model(aiMesh *inmesh)).
 *  @param	min, max	The two corners that define the new box. Idea is that min[i] < max[i] for
 *  						i in {0, 1, 2}, but might work otherwise, too.
 */
bounding_box::bounding_box(glm::vec3 min, glm::vec3 max) {
	position = min;
	glm::vec3 dif = max - min;
	sides[0] = glm::vec3(dif.x, 0, 0);
	sides[1] = glm::vec3(0, dif.y, 0);
	sides[2] = glm::vec3(0, 0, dif.z);
}

/**
 *  Box-initialized constructor. Takes one vertex and three edges of the box and creates a bounding box
 *  with them. If the box is not a Parallelepiped, there might be some inconsistencies...
 *  @param arg_box	The box you want yours to be like.
 */
bounding_box::bounding_box(const box &arg_box) {
	position = arg_box.vertices[0];
	sides[0] = arg_box.vertices[1] - position;
	sides[1] = arg_box.vertices[2] - position;
	sides[2] = arg_box.vertices[4] - position;
}

/**
 *  Constructor specifying a direction. Creates a tiny bounding box at the origin, which is facing in
 *  the given direction. Useful if you nedd an oriented bounding box: You can add vertices with
 *  add_vertex(), and since that only expands the box and doesn't change its orientation, you
 *  will get a full bounding box with the desired orientation. It is used in this way in
 *  cs::std_render_pass::get_light_matrix().
 *  @param	direction	The direction you want your box to point in (with one face)
 */
bounding_box::bounding_box(glm::vec3 direction) {
	glm::mat4 matrix = glm::inverse(glm::lookAt(glm::vec3(0, 0, 0), direction, glm::vec3(0, 1, 0)));
	position = glm::vec3(0, 0, 0);
	sides[0] = glm::vec3(matrix * glm::vec4(0.1f, 0, 0, 0));
	sides[1] = glm::vec3(matrix * glm::vec4(0, 0.1f, 0, 0));
	sides[2] = glm::vec3(matrix * glm::vec4(0, 0, 0.1f, 0));
}

/**
 *  Returns the corner vertices of the bounding box. Can be used to construct bigger bounding boxes
 *  from smaller ones (best done using add_bounding_box()), rendering, physics calculations or
 *  whatever else your heart desires...
 */
box bounding_box::get_vertices() const {
	box ret;
	ret.vertices[0] = position;
	ret.vertices[1] = position + sides[0];
	ret.vertices[2] = position + sides[1];
	ret.vertices[3] = position + sides[0] + sides[1];
	ret.vertices[4] = position + sides[2];
	ret.vertices[5] = position + sides[0] + sides[2];
	ret.vertices[6] = position + sides[1] + sides[2];
	ret.vertices[7] = position + sides[0] + sides[1] + sides[2];
	return ret;
}

/**
 *  Expands the bounding box to include the given vertex. It extrudes the faces that are oriented
 *  towards the new vertex. The orientation of the bounding box is not altered in the process.
 *  @param	vertex	A vertex that you want your box to include.
 */
void bounding_box::add_vertex(glm::vec3 vertex) {
	if(sides[0] == sides[1] || sides[1] == sides[2] || sides[2] == sides[0]) { // In case the box has no volume
		bounding_box(position, vertex);
		return;
	}
	for(unsigned int i = 0; i < 3; ++i) {
		if(glm::length(sides[i]) == 0) {
			if(glm::length(sides[(i + 1) % 3]) != 0 && glm::length(sides[(i + 2) % 3]) != 0) {
				sides[i] = glm::cross(sides[(i + 1) % 3], sides[(i + 2) %3]);
				sides[i] = sides[i] / glm::length(sides[i]) * 0.1f;
			}else if(glm::length(sides[(i + 1) % 3]) != 0) {
				if(sides[(i + 1) % 3].x == 0) {
					sides[i] = glm::vec3(0.1f, 0, 0);
				}else {
					sides[i] = glm::vec3(0, 0.1f, 0);
				}
			}else if(glm::length(sides[(i + 2) % 3]) != 0) {
				if(sides[(i + 2) % 3].x == 0) {
					sides[i] = glm::vec3(0.1f, 0, 0);
				}else {
					sides[i] = glm::vec3(0, 0.1f, 0);
				}
			}else{
				sides[i] = glm::vec3(0.1f, 0, 0);
			}
		}

		glm::vec3 normal = glm::cross(sides[(i + 1) % 3], sides[(i + 2) %3]);
		if(glm::dot(normal, sides[i]) < 0) normal = -normal;

		if(glm::dot(vertex - (position + sides[i]), normal) > 0) {
			float t = glm::dot((vertex - position), normal) / glm::dot(sides[i], normal);
			sides[i] = t * sides[i];
		} else if(glm::dot(vertex - position, normal) < 0) {
			float t = glm::dot((vertex - position), normal) / glm::dot(sides[i], normal);
			position = position + t * sides[i];
			sides[i] = sides[i] * (1 - t);
		}
	}
}

/**
 *  Extends the box to encompass another bounding box. Works in principle like (and internally with)
 *  add_vertex(), just with the bounding vertices of the new box.
 *  @param	arg_box	A box containing space that you want. ANNEX IT!!!
 */
void bounding_box::add_bounding_box(const bounding_box &arg_box) {
	box tmp_box = arg_box.get_vertices();
	for(unsigned int i =0; i < 8; ++i) {
		add_vertex(tmp_box.vertices[i]);
	}
}

/**
 *  Returns a matrix for rendering the contents of the box. It is an orthogonal projection matrix
 *  looking the way of the third edge. The clip planes are (approximately) the faces of the box.
 */
glm::mat4 bounding_box::get_light_matrix() {
	glm::vec3 cam_point = position + 0.5f * sides[0] + 0.5f * sides[1];
	glm::vec3 look_point = cam_point + sides[2];
	float dx = 0.5f * glm::length(sides[0]);
	float dy = 0.5f * glm::length(sides[1]);
	float dz = glm::length(sides[2]);
	return glm::ortho(-dx - 1, dx + 1, -dy - 1, dy + 1, 0.1f, dz + 1) * glm::lookAt(cam_point, look_point, glm::vec3(0, 1, 0));
}

/**
 *  Transforms the box within space with the given matrix. The transformed matrix is returned,
 *  the box in the argument will not be altered.
 *  @param	matrix	The transformation to be applied to the bounding_box. For more information on
 *  					matrices in OpenGL see https://open.gl/transformations.
 *  @param	arg_box	The bounding_box to be transformed.
 */
bounding_box operator * (glm::mat4 matrix, const bounding_box &arg_box) {
	box tmp_box = arg_box.get_vertices();
	for(unsigned int i = 0; i < 8; ++i) {
		tmp_box.vertices[i] = glm::vec3(matrix * glm::vec4(tmp_box.vertices[i], 1.0f));
	}
	return bounding_box(tmp_box);
}
