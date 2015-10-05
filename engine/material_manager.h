//Singleton-class managing all material objects

#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include <vector>
#include <iostream>

#include "shader_manager.h"
#include "material.h"

typedef unsigned int material_id;
const unsigned int invalid_material_id = (unsigned int) -1;

class material_manager {
    public:
        static material_manager &get_instance();
        material_id create_material(std::vector<std::pair<std::string, std::shared_ptr<texture>>> new_textures);
        material_id create_material(std::string uniform_name, std::shared_ptr<texture> tex);
        material_id create_material();
        void add_texture(material_id target_material, std::string uniform_name, std::shared_ptr<texture> tex);
        void set_active_material(material_id active_id);
        bool material_has_texture(material_id request_id);
        void add_material_instance(material_id id);
        void remove_material_instance(material_id id);
        ~material_manager();

    private:
        material_manager();
        std::vector<material *> materials;
};

material_id create_material();
void set_active_material(material_id active_id);
bool material_has_texture(material_id request_id);
void add_material_instance(material_id id);
void remove_material_instance(material_id id);

#endif // MATERIAL_MANAGER_H
