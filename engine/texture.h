//Class used to refer to an OpenGL texture for rendering
//All texture objects should be handled by the texture_manager

#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include "texture_data_source.h"

class texture {
        friend class texture_framebuffer_attachment;

    private:
        GLuint openGL_id;
    public:
        texture(const texture_data_source &source, bool generate_mipmaps = true, bool compress = true,
                GLenum wrap_s = GL_REPEAT, GLenum wrap_t = GL_REPEAT, GLenum mag_filter = GL_LINEAR,
                GLenum min_filter = GL_LINEAR_MIPMAP_LINEAR);

        texture(GLsizei width, GLsizei height, GLenum internalformat, GLenum wrap_s = GL_REPEAT,
                GLenum wrap_t = GL_REPEAT, GLenum mag_filter = GL_LINEAR,
                GLenum min_filter = GL_LINEAR_MIPMAP_LINEAR);

        ~texture();

        void bind();

};

#endif // TEXTURE_H
