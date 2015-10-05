#include "material.h"

material::material() {
    instances = 0;
}

material::~material() {
}

bool material::has_texture() {
    if(textures.size() == 0) {
        return false;
    } else {
        return true;
    }
}

void material::add_texture(std::string uniform_name, std::shared_ptr<texture> tex) {
    textures.push_back(std::pair<std::string, std::shared_ptr<texture>>(uniform_name, tex));
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
            glActiveTexture(GL_TEXTURE0 + i);
            textures[i].second->bind();
        }
    }
}
