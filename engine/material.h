/**
The material class defines a set of textures used to render an object.
Material objects are meant to be handled by the material_manager.
*/

#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "texture_link.h"
#include "shader_manager.h"

class material {
    public:
        material();
        ~material();
        bool has_texture();///< Returns 'true' if the material's list of textures contains one or more texture_links, and 'false' otherwise.
        void add_texture(texture_link* new_texture);///< Adds the given texture_link object to the material's list of textures.
        unsigned int get_instance_count();///< returns the number of references to the material.
        void add_instance();
        void remove_instance();
        void use();///< Gives all associated textures to the currently active shader.

    private:
        std::vector<texture_link*> textures;
        unsigned int instances;
};

#endif // MATERIAL_H
