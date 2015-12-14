/**
The material class defines a set of textures used to render an object.
Material objects are meant to be handled by the material_manager.
*/

#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>

#include "texture.h"
#include "uniforms.h"

class material {
    public:
        typedef std::pair<std::string, std::shared_ptr<texture>> texture_binding;
        typedef std::vector<texture_binding> texture_bindings;

        material();
        material(const texture_bindings &textures);
        void set_uniform(std::string name, uniform::ptr new_uniform);
        void remove_uniform(std::string name);
        void add_texture(std::string uniform_name, std::shared_ptr<texture> tex);
        void use();///< Gives all associated textures to the currently active shader.
        bool is_standard();

    private:
        texture_bindings textures;
        std::map<std::string, std::shared_ptr<uniform>> uniforms;
        
    protected:
        bool mat_is_standard;
};

#endif // MATERIAL_H
