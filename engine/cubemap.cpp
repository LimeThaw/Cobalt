//
// Created by flocke on 06.10.15.
//

#include "cubemap.h"

/**
 *  Secondary constructor. Calls the other constructor with format=GL_RGBA. Used to shorten code since
 *  RGBA is the most common format for textures.
 */
cubemap::cubemap(GLsizei width, GLsizei height, std::string arg_name, GLenum internalformat,
                 GLenum wrap_r, GLenum wrap_s, GLenum wrap_t,
                 GLenum mag_filter, GLenum min_filter) : cubemap(width, height, arg_name, internalformat, GL_RGBA, wrap_r, wrap_s, wrap_t, mag_filter, min_filter) {
}

/**
 *  Constructor of the class. Generates six 2D textures and puts them together in a cubemap.
 *  @param	width			Width of the individual faces in pixels
 *  @param	height			Height of the individual faces in pixels
 *  @param	arg_name		The name of the cubemap object. Has nothing to do with OpenGL.
 *
 *  For more information on the other arguments see [here](https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml "glTexImage2D")
 *  and [here](https://www.opengl.org/sdk/docs/man4/html/glTexParameter.xhtml "glTexParameter"). All arguments after
 *  format have default values assigned.
 */
cubemap::cubemap(GLsizei width, GLsizei height, std::string arg_name, GLenum internalformat, GLenum format,
	    	GLenum wrap_r, GLenum wrap_s, GLenum wrap_t,
	    	GLenum mag_filter, GLenum min_filter) {
    bind();
    for(GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X; face <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; ++face) {
        glTexImage2D(face, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE,
                     nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mag_filter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap_r);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap_t);

	name_manager::get_instance()->insert(arg_name, this);
}

/**
 *  Implementation of texture::bind() making the cubemap current in the GL_TEXTURE_CUBE_MAP context.
 *  For more information have a look at the [OpenGL documentation](https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glBindTexture.xml "glBindTexture").
 */
void cubemap::bind() {
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glBindTexture(GL_TEXTURE_CUBE_MAP, get_openGL_id());
}
