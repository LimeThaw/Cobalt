//
// Created by flocke on 06.10.15.
//

#ifndef COBALT_TEXTURE2D_H
#define COBALT_TEXTURE2D_H

#include "texture.h"
#include "texture_cache.h"

#include "pointer_wrapper.h"

/**
 *  Class representing a 2D texture. This extends the texture class and adds some useful
 *  functionality.
 */
class texture2d : public texture {
    public:
        texture2d(const texture_data_source &source, std::string arg_name = "", bool generate_mipmaps = true, bool compress = true,
                GLenum wrap_s = GL_REPEAT, GLenum wrap_t = GL_REPEAT, GLenum mag_filter = GL_LINEAR,
                GLenum min_filter = GL_LINEAR_MIPMAP_LINEAR);

        texture2d(GLsizei width, GLsizei height, std::string arg_name, GLenum internalformat, GLenum wrap_s = GL_REPEAT,
                GLenum wrap_t = GL_REPEAT, GLenum mag_filter = GL_LINEAR,
                GLenum min_filter = GL_LINEAR_MIPMAP_LINEAR);

		texture2d(GLsizei width, GLsizei height, std::string arg_name, GLenum internalformat, GLenum format, GLenum wrap_s = GL_REPEAT,
                GLenum wrap_t = GL_REPEAT, GLenum mag_filter = GL_LINEAR,
                GLenum min_filter = GL_LINEAR_MIPMAP_LINEAR);

        void bind() override;

        static std::shared_ptr<texture2d> load_file(const char* path, const std::string arg_name = "");
};

typedef pointer_wrapper<texture2d> texture2d_ptr;

#endif //COBALT_TEXTURE2D_H
