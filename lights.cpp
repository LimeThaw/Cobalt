#include "lights.h"

//light
light::light() {
    set_color(glm::vec3(255, 255, 255));
    set_intensity(1.0f);
    color_uniform = "";
}

light::light(glm::vec3 new_color, float new_intensity, const char* new_color_uniform) {
    set_color(new_color);
    set_intensity(new_intensity);
    set_color_uniform(new_color_uniform);
    if(color_uniform == "") std::cerr << "! Created light with empty color uniform\n\n";
}

light::~light() {}

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

void light::set_color_uniform(const char* new_color_uniform) {
    color_uniform = new_color_uniform;
}

void light::apply_color() {
    if(color_uniform != "") {
        GLint shader_id;
        glGetIntegerv(GL_CURRENT_PROGRAM, &shader_id);
        glm::vec3 out_vec = color * intensity / 255.0f;
        glUniform3f(glGetUniformLocation(shader_id, color_uniform), out_vec.x, out_vec.y, out_vec.z);
    } else {
        //std::cerr << "! Tried to apply light without color uniform\n";
        return;
    }
}

//sun_light
sun_light::sun_light() : light() {
    set_direction(glm::vec3(0, 0, -1));
}

sun_light::sun_light(glm::vec3 new_color, float new_intensity, glm::vec3 new_direction, const char* new_color_uniform) {
    light(new_color, new_intensity, new_color_uniform);
    set_direction(new_direction);
}

void sun_light::set_direction(glm::vec3 new_direction) {
    if(glm::length(new_direction) != 0) {
        direction = new_direction;
    } else {
        std::cerr << "! Can't assign a nullvector as direction for a sun light\n";
        return;
    }
}

const glm::vec3 &sun_light::get_direction() {
    return direction;
}

void sun_light::apply() {
    apply_color();
}
