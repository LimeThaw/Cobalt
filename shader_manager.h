//Singleton-class managing all shader objects

#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <vector>

#include "shader.h"

const unsigned int invalid_shader_id = (unsigned int) -1;

class shader_manager {
    public:
        static shader_manager &get_instance();
        unsigned int load_shader(const char *vertex_path, const char *fragment_path);
        void add_shader_instance(unsigned int id);
        void remove_shader_instance(unsigned int id);
        void use_shader(unsigned int id);
        ~shader_manager();

    private:
        shader_manager();
        std::vector<shader *> shaders;
};

int load_global_shader(const char *vertex_path, const char *fragment_path);
void add_shader_instance(unsigned int id);
void remove_shader_instance(unsigned int id);

#endif // SHADER_MANAGER_H
