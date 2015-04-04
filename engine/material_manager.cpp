#include "material_manager.h"

material_manager &material_manager::get_instance() {
    static material_manager instance;
    return instance;
}

material_id material_manager::create_material(std::vector<texture_link*> new_textures){
    material *new_material = new material();
    for(int i=0; i<new_textures.size(); i++){
        new_material->add_texture(new_textures[i]);
    }
    materials.push_back(new_material);
    return materials.size() - 1;
}

material_id material_manager::create_material(texture_link* new_texture) {
    material *new_material = new material();
    new_material->add_texture(new_texture);
    materials.push_back(new_material);
    return materials.size() - 1;
}

material_id material_manager::create_material() {
    material *new_material = new material();
    materials.push_back(new_material);
    return materials.size() - 1;
}

void material_manager::add_texture(material_id target_material, texture_link* new_texture) {
    materials[target_material]->add_texture(new_texture);
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

material_id create_material(std::vector<texture_link*> new_textures){
    return material_manager::get_instance().create_material(new_textures);
}

material_id create_material(texture_link* new_texture) {
    return material_manager::get_instance().create_material(new_texture);
}

material_id create_material() {
    return material_manager::get_instance().create_material();
}

void add_texture(material_id target_material, texture_link* new_texture) {
    material_manager::get_instance().add_texture(target_material, new_texture);
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
