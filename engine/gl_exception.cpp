#include "gl_exception.h"

#include <GL/glu.h>

/**
 *  Class constructor. Initializes an exception used to display an OpenGL error with the given error
 *  code. For information on OpenGL error codes visit https://www.khronos.org/opengl/wiki/OpenGL_Error
 *  @param	error_code	The code of the OpenGL error. The string carried by the exception will
 *  						describe the error in a human-readable way.
 */
gl_exception::gl_exception(GLint error_code) : std::runtime_error(std::string((const char*) gluErrorString(error_code))) {

}

/**
 *  Ignores all current errors. Queries errors from OpenGL and discards results. This resets OpenGLs
 *  error flag to GL_NO_ERROR.
 */
void clear_gl_error() {
    glGetError();
}

/**
 *  Throws exception of any OpenGL errors ocurred since the last query. It retrieves the state of
 *  OpenGLs error flag, creates and throws and exception with a string describing the error. This
 *  function resets the value of the error flag to GL_NO_ERROR (just like clear_gl_error()).
 */
void check_gl_error() {
    GLint error_code = glGetError();
    if(error_code != GL_NO_ERROR) {
        throw gl_exception(error_code);
    }
}
