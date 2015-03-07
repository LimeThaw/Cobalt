//Class used to link a texture to a shader

#ifndef TEXTURE_LINK_H
#define TEXTURE_LINK_H

#include "texture_manager.h"

class texture_link {
    public:
        texture_link(const char* texture_path, const char* uniform_name);
        texture_link(texture_id new_texture, const char* uniform_name);
        ~texture_link();
        void set_texture(const char* path);
        void set_texture(texture_id new_texture);
        void set_uniform(const char* uniform_name);
        void apply(unsigned int place);

    private:
        texture_id linked_texture_id;
        const char* uniform_location;
};

#endif // TEXTURE_LINK_H
