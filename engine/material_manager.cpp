#include "material_manager.h"

material_manager &material_manager::get_instance() {
    static material_manager instance;
    return instance;
}

material_id material_manager::create_material(
        std::vector<std::pair<std::string, std::shared_ptr<texture>>> new_textures) {
    material *new_material = new material();
    for(auto i : new_textures) {
        new_material->add_texture(i.first, i.second);
    }
    materials.push_back(new_material);
    return materials.size() - 1;
}

material_id material_manager::create_material(std::string uniform_name, std::shared_ptr<texture> tex) {
    material *new_material = new material();
    new_material->add_texture(uniform_name, tex);
    materials.push_back(new_material);
    return materials.size() - 1;
}

material_id material_manager::create_material() {
    material *new_material = new material();
    materials.push_back(new_material);
    return materials.size() - 1;
}

void material_manager::add_texture(material_id target_material, std::string uniform_name,
                                   std::shared_ptr<texture> tex) {
    materials[target_material]->add_texture(uniform_name, tex);
}

void material_manager::set_active_material(material_id activeID) {
    materials[activeID]->use();
}

bool material_manager::material_has_texture(material_id requestID) {
    return materials[requestID]->has_texture();
}

void material_manager::add_material_instance(material_id id) {
    materials[id]->add_instance();
}

void material_manager::remove_material_instance(material_id id) {
    materials[id]->remove_instance();
}

material_manager::~material_manager() {
    for(material_id i = 0; i < materials.size(); i++) {
        delete materials[i];
    }
    materials.clear();
}

material_manager::material_manager() {
    //ctor
}

material_id create_material() {
    return material_manager::get_instance().create_material();
}

void set_active_material(material_id active_id) {
    material_manager::get_instance().set_active_material(active_id);
}

bool material_has_texture(material_id request_id) {
    return material_manager::get_instance().material_has_texture(request_id);
}

void add_material_instance(material_id id) {
    material_manager::get_instance().add_material_instance(id);
}

void remove_material_instance(material_id id) {
    material_manager::get_instance().remove_material_instance(id);
}
