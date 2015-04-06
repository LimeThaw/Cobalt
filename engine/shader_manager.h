//Singleton-class managing all shader objects

#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <vector>

#include "shader.h"

typedef unsigned int shader_id;
const unsigned int invalid_shader_id = (unsigned int) -1;

class shader_manager {
    public:
        static shader_manager &get_instance();
        shader_id load_shader(const std::string &vertex_path, const std::string &fragment_path);
        void add_shader_instance(shader_id id);
        void remove_shader_instance(shader_id id);
        void use_shader(shader_id id);
        ~shader_manager();

    private:
        shader_manager();
        std::vector<shader *> shaders;
};

shader_id load_global_shader(const std::string &vertex_path, const std::string &fragment_path);
void add_shader_instance(shader_id id);
void remove_shader_instance(shader_id id);

#endif // SHADER_MANAGER_H
