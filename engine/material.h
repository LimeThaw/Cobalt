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
#include <string>

#include "texture.h"
#include "uniforms.h"
#include "name_manager.h"
#include "json.hpp"
using json = nlohmann::json;

class material {
    public:
        typedef std::pair<std::string, std::shared_ptr<texture>> texture_binding;
        typedef std::vector<texture_binding> texture_bindings;

        material(std::string arg_name = "", bool shadow_caster = true);
        material(const texture_bindings &textures, std::string arg_name = "", bool shadow_caster = true);
        void set_uniform(std::string name, uniform::ptr new_uniform);
        void remove_uniform(std::string name);
        json get_uniforms_json();
        void add_texture(std::string uniform_name, std::shared_ptr<texture> tex);
        const texture_bindings &get_textures();
        void use();///< Gives all associated textures to the currently active shader.
        bool is_standard();
        bool is_shadow_caster();
        void set_shadow_cast(bool shadow);
        string set_name(const string arg_name);
        string get_name();

    private:
        texture_bindings textures;
        std::map<std::string, std::shared_ptr<uniform>> uniforms;
        bool shadow_caster;
        
    protected:
        bool mat_is_standard;
        std::string name;
};

#endif // MATERIAL_H
