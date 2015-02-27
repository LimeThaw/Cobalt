#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

class texture{
    public:
        texture();
        ~texture();
        bool load(const char* path);
        void bind_texture();
        unsigned int get_instance_count();
        void add_instance();
        void remove_instance();

    private:
        GLuint openGL_id;
        unsigned int instances;
};

#endif // TEXTURE_H
