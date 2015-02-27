#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "texture_manager.h"
#include "shader_manager.h"

class material{
    public:
        material();
        ~material();
        bool has_texture();
        void set_texture(unsigned int new_texture);
        void set_shader(unsigned int new_shader);
        unsigned int get_instance_count();
        void add_instance();
        void remove_instance();
        void use();

    private:
        int textureID;
        int shaderID;
        bool texture;
        unsigned int instances;
};

#endif // MATERIAL_H
