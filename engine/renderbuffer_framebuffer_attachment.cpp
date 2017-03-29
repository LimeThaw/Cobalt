//
// Created by flocke on 05.10.15.
//

#include <GL/glew.h>
#include "renderbuffer_framebuffer_attachment.h"

/**
 *  Constructing constructor. Initializes your attachment and everything you need to use it.
 *  More specifically it has OpenGL generate a buffer and keeps the index to it.
 *  @param	internalformat	The format you want your buffer in. For more info on OpenGL's image formats
 *  							see https://www.khronos.org/opengl/wiki/Image_Format
 *  @param	width			The width of the buffer
 *  @param	height			The height of the buffer
 
 *  For more insight about the buffer and the arguments see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glRenderbufferStorage.xhtml
 */
renderbuffer_framebuffer_attachment::renderbuffer_framebuffer_attachment(GLenum internalformat, GLsizei width,
                                                                         GLsizei height) {
    glGenRenderbuffers(1, &openGL_id);
    glBindRenderbuffer(GL_RENDERBUFFER, openGL_id);
    glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
}

/**
 *  Destructor. Has OpenGL free the space allocated to our buffer. This reference should not be used
 *  afterwards - lucky for us we just destroyed the referring object.
 */
renderbuffer_framebuffer_attachment::~renderbuffer_framebuffer_attachment() {
    glDeleteRenderbuffers(1, &openGL_id);
}

/**
 *  Binds the attachment for rendering. For the next render call this buffer will be attached to
 *  the given channel of the output, and hold its data. Basically implements this call:
 *  https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glFramebufferRenderbuffer.xml
 *  @param	target	The channel you want to save in this buffer. Must be GL_COLOR_ATTACHMENT0,
 *  					GL_DEPTH_ATTACHMENT, or GL_STENCIL_ATTACMENT.
 */
void renderbuffer_framebuffer_attachment::attachAs(GLuint target) {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, target, GL_RENDERBUFFER, openGL_id);
}
