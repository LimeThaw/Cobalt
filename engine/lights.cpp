#include "lights.h"

//light
light::light(glm::vec3 new_color, float new_intensity) {
    set_color(new_color);
    set_intensity(new_intensity);
}

void light::set_color(glm::vec3 new_color) {
    color.x = clamp(new_color.x, 0.0f, 1.0f);
    color.y = clamp(new_color.y, 0.0f, 1.0f);
    color.z = clamp(new_color.z, 0.0f, 1.0f);
}

const glm::vec3 &light::get_color() {
    return color;
}

void light::set_intensity(float new_intensity) {
    intensity = new_intensity;
}

float light::get_intensity() {
    return intensity;
}

//directional_light
directional_light::directional_light(glm::vec3 color, float intensity, glm::vec3 direction, string arg_name) : light(color, intensity) {
    set_direction(direction);
    if(arg_name == "") {
    	name = name_manager::get_instance()->insert(this);
    } else {
    	name = name_manager::get_instance()->insert(arg_name, this);
    }
}

void directional_light::set_direction(glm::vec3 new_direction) {
    if(glm::length(new_direction) != 0) {
        direction = glm::normalize(new_direction);
    } else {
        std::cerr << "! Can't assign a nullvector as direction for a sun light\n";
        return;
    }
}

const glm::vec3 &directional_light::get_direction() {
    return direction;
}

string directional_light::set_name(const string arg_name) {
	name_manager::get_instance()->remove(name);
	name = name_manager::get_instance()->insert(arg_name, this);
	return name;
}

string directional_light::get_name() {
	return name;
}

//point_light
point_light::point_light(glm::vec3 color, float intensity, glm::vec3 position, float radius, string arg_name) : light(color, intensity), node(), radius(radius) {
    place(position);
    set_name(arg_name);
}

const glm::vec3 point_light::get_position() {
    return glm::vec3((node_matrix * glm::vec4(0, 0, 0, 1)));
}

float point_light::get_radius() {
    return radius;
}

