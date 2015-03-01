#ifndef GL_EXCEPTION_H
#define GL_EXCEPTION_H

#include <stdexcept>
#include <GL/gl.h>

class gl_exception :
    public std::runtime_error {
        public:
            gl_exception(GLint error_code);
};

//Cleanup after external function call
void clear_gl_error();

void check_gl_error();

#endif