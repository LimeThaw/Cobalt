//Class used to link a texture to a shader

#ifndef TEXTURE_LINK_H
#define TEXTURE_LINK_H

#include "texture_manager.h"

const GLuint invalid_uniform_id = (GLuint) -1;

class texture_link {
    public:
        texture_link();
        texture_link(const char* texture_path, const char* uniform_name);
        texture_link(unsigned int new_texture, const char* uniform_name);
        ~texture_link();
        void set_texture(const char* path);
        void set_texture(unsigned int new_texture);
        void set_uniform(const char* uniform_name);
        void apply(unsigned int place);

    private:
        unsigned int texture_id;
        const char* uniform_location;
};

#endif // TEXTURE_LINK_H
