#include "shader_manager.h"

shader_manager &shader_manager::get_instance() {
    static shader_manager instance;
    return instance;
}

shader_id shader_manager::load_shader(const std::string &vertex_path, const std::string &fragment_path) {
    shader *new_shader = new shader();
    new_shader->load_shader(vertex_path, fragment_path);
    shaders.push_back(new_shader);
    return shaders.size() - 1;
}

void shader_manager::add_shader_instance(shader_id id) {
    if(shaders.size() <= id || shaders[id] == NULL) {
        std::cerr << "! Can't add instance of shader [" << id << "] - shader doesn't exist\n";
    } else {
        shaders[id]->add_instance();
    }
}

void shader_manager::remove_shader_instance(shader_id id) {
    for(unsigned int i = 0; i < shaders.size(); i++) {
        if(shaders[i]->get_shader_id() == id) {
            shaders[i]->remove_instance();
            break;
        }
    }
}

void shader_manager::use_shader(shader_id id) {
    shaders[id]->use();
}

shader_manager::~shader_manager() {
    //dtor
}

shader_manager::shader_manager() {
    //ctor
}

shader_id load_global_shader(const std::string &vertex_path, const std::string &fragment_path) {
    return shader_manager::get_instance().load_shader(vertex_path, fragment_path);
}

void add_shader_instance(shader_id id) {
    shader_manager::get_instance().add_shader_instance(id);
}

void remove_shader_instance(shader_id id) {
    shader_manager::get_instance().remove_shader_instance(id);
}
