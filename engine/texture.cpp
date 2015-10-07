#include "texture.h"
#include "gl_exception.h"
#include "util.h"
#include <glm/glm.hpp>


texture::texture() {
    glGenTextures(1, &openGL_id);
}

texture::~texture() {
    glDeleteTextures(1, &openGL_id);
}

GLuint texture::get_openGL_id() {
    return openGL_id;
}
