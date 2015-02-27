#include "material_manager.h"

material_manager& material_manager::get_instance(){
    static material_manager instance;
    return instance;
}

unsigned int material_manager::create_material(unsigned int arg_shaderID, unsigned int arg_textureID){
    material* new_material = new material();
    new_material->set_shader(arg_shaderID);
    new_material->set_texture(arg_textureID);
    materials.push_back(new_material);
    return materials.size()-1;
}

unsigned int material_manager::create_material(unsigned int arg_shaderID){
    material* new_material = new material();
    new_material->set_shader(arg_shaderID);
    materials.push_back(new_material);
    return materials.size()-1;
}

void material_manager::set_active_material(unsigned int activeID){
    materials[activeID]->use();
}

bool material_manager::material_has_texture(unsigned int requestID){
    return materials[requestID]->has_texture();
}

void material_manager::add_material_instance(unsigned int id){
    materials[id]->add_instance();
}

void material_manager::remove_material_instance(unsigned int id){
    materials[id]->remove_instance();
}

material_manager::~material_manager(){
    for(int i=0; i<materials.size(); i++){
        delete materials[i];
    }
    materials.clear();
}

material_manager::material_manager(){
    //ctor
}

unsigned int create_material(unsigned int arg_shaderID, unsigned int arg_textureID){
    material_manager::get_instance().create_material(arg_shaderID, arg_textureID);
}

unsigned int create_material(unsigned int arg_shaderID){
    material_manager::get_instance().create_material(arg_shaderID);
}

void set_active_material(unsigned int activeID){
    material_manager::get_instance().set_active_material(activeID);
}

bool material_has_texture(unsigned int requestID){
    return material_manager::get_instance().material_has_texture(requestID);
}

void add_material_instance(unsigned int id){
    material_manager::get_instance().add_material_instance(id);
}

void remove_material_instance(unsigned int id){
    material_manager::get_instance().remove_material_instance(id);
}
