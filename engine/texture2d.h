//
// Created by flocke on 06.10.15.
//

#ifndef COBALT_TEXTURE2D_H
#define COBALT_TEXTURE2D_H


#include "texture.h"

class texture2d :
    public texture {
    public:
        texture2d(const texture_data_source &source, bool generate_mipmaps = true, bool compress = true,
                GLenum wrap_s = GL_REPEAT, GLenum wrap_t = GL_REPEAT, GLenum mag_filter = GL_LINEAR,
                GLenum min_filter = GL_LINEAR_MIPMAP_LINEAR);

        texture2d(GLsizei width, GLsizei height, GLenum internalformat, GLenum wrap_s = GL_REPEAT,
                GLenum wrap_t = GL_REPEAT, GLenum mag_filter = GL_LINEAR,
                GLenum min_filter = GL_LINEAR_MIPMAP_LINEAR);

        void bind() override;
};


#endif //COBALT_TEXTURE2D_H
