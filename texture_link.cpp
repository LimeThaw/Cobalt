#include "texture_link.h"

texture_link::texture_link() {
    texture_id = invalid_texture_id;
    texture_id = invalid_uniform_id;
}

texture_link::texture_link(const char* texture_path, const char* uniform_name) {
    set_texture(texture_path);
    set_uniform(uniform_name);
}

texture_link::texture_link(unsigned int new_texture, const char* uniform_name) {
    set_texture(new_texture);
    set_uniform(uniform_name);
}

texture_link::~texture_link() {
    if(texture_id != invalid_texture_id) remove_texture_instance(texture_id);
}

void texture_link::set_texture(const char* path) {
    texture_id = load_global_texture(path);
    add_texture_instance(texture_id);
}

void texture_link::set_texture(unsigned int new_texture) {
    texture_id = new_texture;
    add_texture_instance(texture_id);
}

void texture_link::set_uniform(const char* uniform_name) {
    GLint shader_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &shader_id);      //PROBLEM: User has to use() shader first
    uniform_location = glGetUniformLocation(shader_id, uniform_name);
}


void texture_link::apply(unsigned int place) {
    if(place <= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {
        glActiveTexture(GL_TEXTURE0 + place);
        texture_manager::get_instance().bind_texture(texture_id);
        glUniform1i(uniform_location, place);
    }else {
        std::cerr << "! Tried to load more textures than can be handled\n";
        return;
    }
}