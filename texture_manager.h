#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <vector>

#include "texture.h"

class texture_manager{
    public:
        static texture_manager& get_instance();
        unsigned int load_texture(const char* texture_path);
        void add_texture_instance(unsigned int id);
        void remove_texture_instance(unsigned int id);
        void bind_texture(unsigned int id);
        void unbind_texture();
        ~texture_manager();

    private:
        texture_manager();
        std::vector<texture*> textures;
};

unsigned int load_global_texture(const char* path);
void add_texture_instance(unsigned int id);
void remove_texture_instance(unsigned int id);

#endif // TEXTURE_MANAGER_H
