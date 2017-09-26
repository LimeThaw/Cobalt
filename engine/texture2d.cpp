//
// Created by flocke on 06.10.15.
//

#include "texture2d.h"
#include "gl_exception.h"

/**
 *  Constructor creating a texture object from a pre-loaded image. The image is represented as a
 *  texture_data_source.
 *  @param	source			The data source for the image that is loaded into the texture.
 *  @param	arg_name		The name you want to give your new texture. You will later be able
 *  							to retrieve it with this name. See name_manager for more detail.
 *  @param	generate_mipmaps	If this is set to true, OpenGL will generate mipmaps for the
 *  								new texture.
 *  @param	compress		No idea...
 *  @param	wrap_s, wrap_t	Wrapping behavior of the texture
 *  @param mag_filter, min_filter	Filtering behavior of the texture
 *
 *  For more info on texture parameters see
 *  https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glTexParameter.xml
 */
texture2d::texture2d(const texture_data_source &source, std::string arg_name, bool generate_mipmaps, bool compress, GLenum wrap_s, GLenum wrap_t,
                 GLenum mag_filter, GLenum min_filter) {
    bind();
    if((bool) source.data) {
        //SOIL_create_OGL_texture((const unsigned char *) source.data.get(), source.width,
		//	source.height, source.num_channels, get_openGL_id(), flags);
		glTexImage2D(GL_TEXTURE_2D, 0, source.format, source.width, source.height, 0, source.format,
			GL_UNSIGNED_BYTE, (const unsigned char *) source.data.get());
		if(generate_mipmaps) glGenerateMipmap(GL_TEXTURE_2D);
        clear_gl_error();
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, source.format, source.width, source.height, 0, source.format,
                     source.type, nullptr);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);

	set_name(arg_name);
	path = texture_cache::get_instance().get_path(source);
}

/**
 *  Convenience overload, because the most common format is GL_RGBA.
 */
texture2d::texture2d(GLsizei width, GLsizei height, std::string arg_name, GLenum internalformat, GLenum wrap_s, GLenum wrap_t, GLenum mag_filter,
                 GLenum min_filter) : texture2d(width, height, arg_name, internalformat, GL_RGBA, wrap_s, wrap_t, mag_filter, min_filter) {
}

/**
 *  Constructor creating a new, empty texture. It tells OpenGL to reserve space for a
 *  texture of the given size, but doesn't populate it.'
 *  @param	width, height	The dimensions of the new texture in pixels.
 *  @param	arg_name		The name you want to give your new texture. You will later be able
 *  							to retrieve it with this name. See name_manager for more detail.
 *  @param	internalformat	A hint for OpenGL what storage format to use. See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml#id-1.6.13.1
 *  							for options.
 *  @param	format			The channels you want your texture to have. Options are GL_RED,
 *  							GL_RG, GL_RGB, GL_RGBA, GL_DEPTH_COMPONENT and GL_DEPTH_STENCIL.
 *  @param	wrap_s, wrap_t	Wrapping behavior of the texture
 *  @param mag_filter, min_filter	Filtering behavior of the texture
 *
 *  For more info on texture parameters see
 *  https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glTexParameter.xml
 */

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

	set_name(arg_name);
}

/**
 *  Sets this texture as the active 2D texture in OpenGL. This means that all actions operating
 *  on GL_TEXTURE_2D now operate on this texture.
 */
void texture2d::bind() {
    glBindTexture(GL_TEXTURE_2D, get_openGL_id());
}

/**
 *  Loads a texture from a file. The image is loaded and cached, a new texture is created with
 *  the given name, and a shared_prt to it is returned.
 *  @param	path		The path to the file that holds the image.
 *  @param	arg_name	The name you want to give your new texture.
 */
std::shared_ptr<texture2d> texture2d::load_file(const char* path, const std::string arg_name) {
	return std::make_shared<texture2d>(texture_cache::get_instance().get_texture_from_filename(path), arg_name);
}