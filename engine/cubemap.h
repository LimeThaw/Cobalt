//
// Created by flocke on 06.10.15.
//

#ifndef COBALT_CUBEMAP_H
#define COBALT_CUBEMAP_H


#include "texture.h"
#include "pointer_wrapper.h"

/**
 *  Class representing a cubemap texture. Holds the OpenGL id, the name and the source path of the
 *  texture. It is an extension to the texture class with the purpose of making the code easier to read,
 *  shorter and more well-structured.
 */
class cubemap :
    public texture {
    public:
        cubemap(GLsizei width, GLsizei height, std::string arg_name, GLenum internalformat,
		    GLenum wrap_r = GL_CLAMP_TO_EDGE, GLenum wrap_s = GL_CLAMP_TO_EDGE, GLenum wrap_t = GL_CLAMP_TO_EDGE,
		    GLenum mag_filter = GL_LINEAR, GLenum min_filter = GL_LINEAR);

	    cubemap(GLsizei width, GLsizei height, std::string arg_name, GLenum internalformat, GLenum format,
	    	GLenum wrap_r = GL_CLAMP_TO_EDGE, GLenum wrap_s = GL_CLAMP_TO_EDGE, GLenum wrap_t = GL_CLAMP_TO_EDGE,
	    	GLenum mag_filter = GL_LINEAR, GLenum min_filter = GL_LINEAR);

        void bind() override;
};

typedef pointer_wrapper<cubemap> cubemap_ptr;


#endif //COBALT_CUBEMAP_H
