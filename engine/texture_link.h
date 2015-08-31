/**
texture_link objects are used to link a texture to a shader by linking the name of the shader's matching uniform to the texture.
*/

#ifndef TEXTURE_LINK_H
#define TEXTURE_LINK_H

#include "texture_manager.h"

class texture_link {
    public:
        texture_link(const std::string &texture_path, const std::string &uniform_name);///< Load a new texture and create a texture_link for it.
        texture_link(texture_id new_texture, const std::string &uniform_name);///< Create a texture_link for a texture that is already loaded.
        ~texture_link();
        void set_texture(const std::string &path);///< Load a new texture and link it to the uniform.
        void set_texture(texture_id new_texture);///< Link a texture to the uniform that has already been loaded.
        void set_uniform(const std::string &uniform_name);///< Defines a new uniform name to link the texture to.
        void apply(unsigned int place);///< Tell the active shader to use the specified texture in the matching uniform.

    private:
        texture_id linked_texture_id;
        std::string uniform_location;
};

#endif // TEXTURE_LINK_H
