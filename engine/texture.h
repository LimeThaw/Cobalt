//Class used to refer to an OpenGL texture for rendering
//All texture objects should be handled by the texture_manager

#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

class texture {
    public:
        texture();
        ~texture();
        bool load(const std::string &path);
        void load_normalmap_from_heightmap(const std::string &path);
        void bind_texture();
        unsigned int get_instance_count();
        void add_instance();
        void remove_instance();

    private:
        GLuint openGL_id;
        unsigned int instances;
        
        static int get_num_mipmap_levels(int width, int height);
};

#endif // TEXTURE_H
