#include "material.h"

material::material() {
    texture_id = invalid_texture_id;
    shader_id = invalid_shader_id;
    instances = 0;
}

material::~material() {
    if(texture_id != invalid_texture_id) {
        remove_texture_instance(texture_id);
    }
    if(shader_id != invalid_shader_id) {
        remove_shader_instance(shader_id);
    }
}

bool material::has_texture() {
    return texture_id != invalid_texture_id;
}

void material::set_texture(unsigned int new_texture) {
    if(texture_id != invalid_texture_id) {
        remove_texture_instance(texture_id);
    }
    texture_id = new_texture;
    add_texture_instance(texture_id);
}

void material::set_shader(unsigned int new_shader) {
    if(shader_id != invalid_shader_id) {
        remove_shader_instance(shader_id);
    }
    shader_id = new_shader;
    add_shader_instance(shader_id);
}

unsigned int material::get_instance_count() {
    return instances;
}

void material::add_instance() {
    instances++;
}

void material::remove_instance() {
    instances--;
}

void material::use() {
    if(shader_id != invalid_shader_id) {
        shader_manager::get_instance().use_shader(shader_id);
    } else {
        std::cerr << "! Tried to use a material withoud shader\n";
    }
    if(texture_id != invalid_texture_id) {
        texture_manager::get_instance().bind_texture(texture_id);
    }
}
