#include "texture.h"

texture::texture(std::string arg_name) {
    glGenTextures(1, &openGL_id);
	name = name_manager::get_instance()->insert(arg_name, this);
	path = "";
}

texture::~texture() {
    glDeleteTextures(1, &openGL_id);
    name_manager::get_instance()->remove(name);
}

GLuint texture::get_openGL_id() {
    return openGL_id;
}

std::string texture::set_name(const std::string arg_name) {
	name_manager::get_instance()->remove(name);
	name = name_manager::get_instance()->insert(arg_name, this);
	return name;
}

std::string texture::get_name() {
	return name;
}

std::string texture::get_path() {
	return path;
}
