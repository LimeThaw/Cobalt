//Class used to refer to an OpenGL texture for rendering
//All texture objects should be handled by the texture_manager

#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

class texture {
    friend class texture_framebuffer_attachment;
    public:
        texture();
        ~texture();
        bool load(const std::string &path);
        void defineStorage(GLenum internalformat, GLsizei width, GLsizei height);
        void bind_texture();
        unsigned int get_instance_count();
        void add_instance();
        void remove_instance();
    private:
        GLuint openGL_id;
        unsigned int instances;
};

#endif // TEXTURE_H
