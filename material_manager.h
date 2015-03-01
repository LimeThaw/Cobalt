//Singleton-class managing all material objects

#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include <vector>
#include <iostream>

#include "texture_manager.h"
#include "shader_manager.h"
#include "material.h"

class material_manager {
    public:
        static material_manager &get_instance();
        unsigned int create_material(unsigned int arg_shader_id, unsigned int arg_texture_id);
        unsigned int create_material(unsigned int arg_shader_id);
        void set_active_material(unsigned int active_id);
        bool material_has_texture(unsigned int request_id);
        void add_material_instance(unsigned int id);
        void remove_material_instance(unsigned int id);
        ~material_manager();

    private:
        material_manager();
        std::vector<material *> materials;
};

unsigned int create_material(unsigned int arg_shader_id, unsigned int arg_texture_id);
unsigned int create_material(unsigned int arg_shader_id);
void set_active_material(unsigned int active_id);
bool material_has_texture(unsigned int request_id);
void add_material_instance(unsigned int id);
void remove_material_instance(unsigned int id);

#endif // MATERIAL_MANAGER_H