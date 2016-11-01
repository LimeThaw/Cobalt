#include "texture.h"

texture::texture(std::string arg_name) : named(arg_name) {
    glGenTextures(1, &openGL_id);
	path = "";
}

texture::~texture() {
    glDeleteTextures(1, &openGL_id);
}

GLuint texture::get_openGL_id() {
    return openGL_id;
}

std::string texture::get_path() {
	return path;
}
