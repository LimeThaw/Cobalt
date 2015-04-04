#include "texture_link.h"

texture_link::texture_link(const char* texture_path, const char* uniform_name) {
    set_texture(texture_path);
    set_uniform(uniform_name);
}

texture_link::texture_link(unsigned int new_texture, const char* uniform_name) {
    set_texture(new_texture);
    set_uniform(uniform_name);
}

texture_link::~texture_link() {
    if(linked_texture_id != invalid_texture_id) remove_texture_instance(linked_texture_id);
}

void texture_link::set_texture(const char* path) {
    linked_texture_id = load_global_texture(path);
    add_texture_instance(linked_texture_id);
}

void texture_link::set_texture(unsigned int new_texture) {
    linked_texture_id = new_texture;
    add_texture_instance(linked_texture_id);
}

void texture_link::set_uniform(const char* uniform_name) {
    uniform_location = uniform_name;
}


void texture_link::apply(unsigned int place) {
    if(place <= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {
        GLint shader_id;
        glGetIntegerv(GL_CURRENT_PROGRAM, &shader_id);

        glActiveTexture(GL_TEXTURE0 + place);
        texture_manager::get_instance().bind_texture(linked_texture_id);
        glUniform1i(glGetUniformLocation(shader_id, uniform_location), place);
    }else {
        std::cerr << "! Tried to load more textures than can be handled\n";
        return;
    }
}
