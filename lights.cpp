#include "lights.h"

//light
light::light(glm::vec3 new_color, float new_intensity) {
    set_color(new_color);
    set_intensity(new_intensity);
}

void light::set_color(glm::vec3 new_color) {
    color.x = clamp(new_color.x, 0.0f, 255.0f);
    color.y = clamp(new_color.y, 0.0f, 255.0f);
    color.z = clamp(new_color.z, 0.0f, 255.0f);
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
directional_light::directional_light(glm::vec3 color, float intensity, glm::vec3 direction) : light(color, intensity) {
    set_direction(direction);
}

void directional_light::set_direction(glm::vec3 new_direction) {
    if(glm::length(new_direction) != 0) {
        direction = new_direction;
    } else {
        std::cerr << "! Can't assign a nullvector as direction for a sun light\n";
        return;
    }
}

const glm::vec3 &directional_light::get_direction() {
    return direction;
}

//point_light
point_light::point_light(glm::vec3 color, float intensity, glm::vec3 position) : light(color, intensity) {
    set_position(position);
}

void point_light::set_position(glm::vec3 new_position) {
    position = new_position;
}

const glm::vec3 &point_light::get_position() {
    return position;
}
