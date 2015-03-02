#include "material.h"

material::material() {
    shader_id = invalid_shader_id;
    instances = 0;
}

material::~material() {
    for(int i=0; i<textures.size(); i++){
        delete textures[i];
    }
    textures.clear();
}

bool material::has_texture() {
    if(textures.size() == 0) {
        return false;
    }else {
        return true;
    }
}

void material::add_texture(texture_link* new_texture) {
    textures.push_back(new_texture);
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
        return;
    }

    if(has_texture()) {
        for(int i=0; i<textures.size(); i++) {
            textures[i]->apply(i);
        }
    }
}
