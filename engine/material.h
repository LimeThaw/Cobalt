/**
The material class defines a set of textures used to render an object.
Material objects are meant to be handled by the material_manager.
*/

#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader_manager.h"
#include "texture.h"

class material {
    public:
        material();
        ~material();
        unsigned int get_instance_count();///< returns the number of references to the material.
        bool has_texture();
        void add_texture(std::string uniform_name, std::shared_ptr<texture> tex);
        void add_instance();
        void remove_instance();
        void use();///< Gives all associated textures to the currently active shader.

    private:
        std::vector<std::pair<std::string, std::shared_ptr<texture>>> textures;
        unsigned int instances;
};

#endif // MATERIAL_H
