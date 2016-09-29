//
// Created by flocke on 06.10.15.
//

#include "texture2d.h"
#include "gl_exception.h"

texture2d::texture2d(const texture_data_source &source, std::string arg_name, bool generate_mipmaps, bool compress, GLenum wrap_s, GLenum wrap_t,
                 GLenum mag_filter, GLenum min_filter) {
    bind();
    if((bool) source.data) {
        uint flags = SOIL_FLAG_INVERT_Y;
        if(generate_mipmaps) {
            flags |= SOIL_FLAG_MIPMAPS;
        }
        if(compress) {
            flags |= SOIL_FLAG_COMPRESS_TO_DXT;
        }
        SOIL_create_OGL_texture((const unsigned char *) source.data.get(), source.width, source.height, source.num_channels,
                                get_openGL_id(), flags);
        clear_gl_error();
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, source.format, source.width, source.height, 0, source.format,
                     source.type, nullptr);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    
	name = name_manager::get_instance()->insert(arg_name, this);
	path = texture_cache::get_instance().get_path(source);
}

texture2d::texture2d(GLsizei width, GLsizei height, std::string arg_name, GLenum internalformat, GLenum wrap_s, GLenum wrap_t, GLenum mag_filter,
                 GLenum min_filter) : texture2d(width, height, arg_name, internalformat, GL_RGBA, wrap_s, wrap_t, mag_filter, min_filter) {
}

texture2d::texture2d(GLsizei width, GLsizei height, std::string arg_name, GLenum internalformat, GLenum format, GLenum wrap_s, GLenum wrap_t, GLenum mag_filter,
                 GLenum min_filter) {
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    
	name = name_manager::get_instance()->insert(arg_name, this);
}

void texture2d::bind() {
    glBindTexture(GL_TEXTURE_2D, get_openGL_id());
}

std::shared_ptr<texture2d> texture2d::load_file(const char* path, const std::string arg_name) {
	return std::make_shared<texture2d>(texture_cache::get_instance().get_texture_from_filename(path), arg_name);
}
