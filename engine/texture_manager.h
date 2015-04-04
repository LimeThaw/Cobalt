/**
Singleton-class managing all texture objects.
*/

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <vector>

#include "texture.h"

typedef unsigned int texture_id;
const unsigned int invalid_texture_id = (unsigned int) -1;

class texture_manager {
    public:
        static texture_manager &get_instance();///< Returns a reference to the texture_manager object.
        texture_id load_texture(const char *texture_path);///< Loads a texture from an image file and returns the texture's texture_id.
        void add_texture_instance(texture_id id);
        void remove_texture_instance(texture_id id);
        void bind_texture(texture_id id);///<Tells OpenGL to bind the specified texture.
        void unbind_texture();
        ~texture_manager();

    private:
        texture_manager();
        std::vector<texture *> textures;
};

texture_id load_global_texture(const char *path);
void add_texture_instance(texture_id id);
void remove_texture_instance(texture_id id);

#endif // TEXTURE_MANAGER_H
