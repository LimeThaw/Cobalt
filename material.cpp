#include "material.h"

material::material() {
    instances = 0;
}

material::~material() {
    for(unsigned int i = 0; i < textures.size(); i++) {
        delete textures[i];
    }
    textures.clear();
}

bool material::has_texture() {
    if(textures.size() == 0) {
        return false;
    } else {
        return true;
    }
}

void material::add_texture(texture_link *new_texture) {
    textures.push_back(new_texture);
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
    if(has_texture()) {
        for(unsigned int i = 0; i < textures.size(); i++) {
            textures[i]->apply(i);
        }
    }
}
