#include "material.h"

material::material(){
    textureID = shaderID = -1;
    texture = false;
    instances = 0;
}

material::~material(){
    remove_texture_instance(textureID);
    remove_shader_instance(shaderID);
}

bool material::has_texture(){
    return texture;
}

void material::set_texture(unsigned int new_texture){
    if(textureID != -1)remove_texture_instance(textureID);
    textureID = new_texture;
    add_texture_instance(textureID);
    texture = true;
}

void material::set_shader(unsigned int new_shader){
    if(shaderID !=-1)remove_shader_instance(shaderID);
    shaderID = new_shader;
    add_shader_instance(shaderID);
}

unsigned int material::get_instance_count(){
    return instances;
}

void material::add_instance(){
    instances++;
}

void material::remove_instance(){
    instances--;
}

void material::use(){
    if(shaderID != -1){
        shader_manager::get_instance().use_shader(shaderID);
    }else{
        std::cerr << "! Tried to use a material withoud shader\n";
    }
    if(textureID != -1 && texture != false){
        texture_manager::get_instance().bind_texture(textureID);
    }
}
