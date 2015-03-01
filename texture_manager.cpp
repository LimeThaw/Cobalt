#include "texture_manager.h"

texture_manager &texture_manager::get_instance() {
    static texture_manager instance;
    return instance;
}

unsigned int texture_manager::load_texture(const char *texture_path) {
    texture *tex = new texture();
    tex->load(texture_path);
    textures.push_back(tex);
    return textures.size() - 1;
}

void texture_manager::add_texture_instance(unsigned int id) {
    if(textures.size() <= id || textures[id] == NULL) {
        std::cerr << "! Can't add instance of texture [" << id << "] - texture doesn't exist\n";
    } else {
        textures[id]->add_instance();
    }
}

void texture_manager::remove_texture_instance(unsigned int id) {
    textures[id]->remove_instance();
}

void texture_manager::bind_texture(unsigned int id) {
    textures[id]->bind_texture();
}

void texture_manager::unbind_texture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

texture_manager::~texture_manager() {
    for(unsigned int i = 0; i < textures.size(); i++) {
        delete textures[i];
    }
    textures.clear();
}

texture_manager::texture_manager() {
    //ctor
}

unsigned int load_global_texture(const char *path) {
    return texture_manager::get_instance().load_texture(path);
}

void add_texture_instance(unsigned int id) {
    texture_manager::get_instance().add_texture_instance(id);
}

void remove_texture_instance(unsigned int id) {
    texture_manager::get_instance().remove_texture_instance(id);
}
