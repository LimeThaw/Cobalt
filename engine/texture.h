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
    protected:
        GLuint get_openGL_id();
    public:
        texture();

        virtual ~texture();

        virtual void bind() = 0;
};

#endif // TEXTURE_H
