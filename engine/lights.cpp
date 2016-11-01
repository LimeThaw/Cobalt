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
directional_light::directional_light() : directional_light(glm::vec3(0), 0, glm::vec3(0, 0, 1), "") {

}

directional_light::directional_light(glm::vec3 color, float intensity, glm::vec3 direction,
		string name) : light(color, intensity), named(name) {
    set_direction(direction);
}

directional_light::~directional_light() {

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

