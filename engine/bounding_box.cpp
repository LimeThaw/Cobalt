#include "bounding_box.h"

bounding_box::bounding_box() {
	bounding_box(glm::vec3(0.0f), glm::vec3(0.0f));
}

bounding_box::bounding_box(glm::vec3 min, glm::vec3 max) {
	position = min;
	glm::vec3 dif = max - min;
	sides[0] = glm::vec3(dif.x, 0, 0);
	sides[1] = glm::vec3(0, dif.y, 0);
	sides[2] = glm::vec3(0, 0, dif.z);
}

bounding_box::bounding_box(box arg_box) {
	position = arg_box.vertices[0];
	sides[0] = arg_box.vertices[1] - position;
	sides[1] = arg_box.vertices[2] - position;
	sides[2] = arg_box.vertices[4] - position;
}

bounding_box::bounding_box(glm::vec3 direction) {
	glm::mat4 matrix = glm::inverse(glm::lookAt(glm::vec3(0, 0, 0), direction, glm::vec3(0, 1, 0)));
	position = glm::vec3(0, 0, 0);
	sides[0] = glm::vec3(matrix * glm::vec4(0.1f, 0, 0, 0));
	sides[1] = glm::vec3(matrix * glm::vec4(0, 0.1f, 0, 0));
	sides[2] = glm::vec3(matrix * glm::vec4(0, 0, 0.1f, 0));
}

box bounding_box::get_vertices() {
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

void bounding_box::add_vertex(glm::vec3 vertex) {
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

void bounding_box::add_bounding_box(bounding_box arg_box) {
	box tmp_box = arg_box.get_vertices();
	for(unsigned int i =0; i < 8; ++i) {
		add_vertex(tmp_box.vertices[i]);
	}
}

glm::mat4 bounding_box::get_light_matrix() {
	glm::vec3 cam_point = position + 0.5f * sides[0] + 0.5f * sides[1];
	glm::vec3 look_point = cam_point + sides[2];
	float dx = 0.5f * glm::length(sides[0]);
	float dy = 0.5f * glm::length(sides[1]);
	float dz = glm::length(sides[2]);
	return glm::ortho(-dx - 1, dx + 1, -dy - 1, dy + 1, 0.1f, dz + 1) * glm::lookAt(cam_point, look_point, glm::vec3(0, 1, 0));
}

bounding_box operator * (glm::mat4 matrix, bounding_box arg_box) {
	box tmp_box = arg_box.get_vertices();
	for(unsigned int i = 0; i < 8; ++i) {
		tmp_box.vertices[i] = glm::vec3(matrix * glm::vec4(tmp_box.vertices[i], 1.0f));
	}
	return bounding_box(tmp_box);
}
