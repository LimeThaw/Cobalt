#include "texture.h"
#include "gl_exception.h"


texture::texture(const texture_data_source &source, bool generate_mipmaps, bool compress, GLenum wrap_s, GLenum wrap_t,
                 GLenum mag_filter, GLenum min_filter) {
    glGenTextures(1, &openGL_id);
    glBindTexture(GL_TEXTURE_2D, openGL_id);

    if((bool) source.data) {
        uint flags = SOIL_FLAG_INVERT_Y;
        if(generate_mipmaps) {
            flags |= SOIL_FLAG_MIPMAPS;
        }
        if(compress) {
            flags |= SOIL_FLAG_COMPRESS_TO_DXT;
        }
        SOIL_create_OGL_texture((const unsigned char *) source.data.get(), source.width, source.height, source.num_channels,
                                openGL_id, flags);
        clear_gl_error();
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, source.format, source.width, source.height, 0, source.format,
                     source.type, nullptr);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
}

texture::texture(GLsizei width, GLsizei height, GLenum internalformat, GLenum wrap_s, GLenum wrap_t, GLenum mag_filter,
                 GLenum min_filter) {
    glGenTextures(1, &openGL_id);
    glBindTexture(GL_TEXTURE_2D, openGL_id);
    glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
}

texture::~texture() {
    glDeleteTextures(1, &openGL_id);
}

void texture::bind() {
    glBindTexture(GL_TEXTURE_2D, openGL_id);
}