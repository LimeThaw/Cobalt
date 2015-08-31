#include "gl_exception.h"

#include <GL/glu.h>

gl_exception::gl_exception(GLint error_code) : std::runtime_error(std::string((const char*) gluErrorString(error_code))) {

}

void clear_gl_error() {
    glGetError();
}

void check_gl_error() {
    GLint error_code = glGetError();
    if(error_code != GL_NO_ERROR) {
        throw gl_exception(error_code);
    }
}
