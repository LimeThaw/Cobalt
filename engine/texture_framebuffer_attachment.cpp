//
// Created by flocke on 05.10.15.
//

#include <GL/glew.h>
#include "texture_framebuffer_attachment.h"

/**
 *  Constructing constructor. Makes sure it knows where to find the texture and where to put it, or,
 *  in other words, what kind of texture it has to deal with.
 *  @param	tex			The texture that you want to render to - Initialize it first!
 *  @param	textarget	The type of texture you have. must be GL_TEXTURE_2D or a cubemap face (
 *  						https://www.khronos.org/opengl/wiki/Cubemap_Texture)
 */
texture_framebuffer_attachment::texture_framebuffer_attachment(std::shared_ptr<texture> tex, GLenum textarget) : tex(
        tex), textarget(textarget) {
}

/**
 *  Destructor. Does nothing, but hey, at least it showed up.
 */
texture_framebuffer_attachment::~texture_framebuffer_attachment() {
}

/**
 *  Attaches the texture for rendering. The next draw call will render the requested channel to the
 *  texture. Basically implements this call:
 *  https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glFramebufferTexture2D.xml
 *  @param	target	Which channel you want to render to your texture. Must be GL_COLOR_ATTACHMENT0,
 *  					GL_DEPTH_ATTACHMENT, or GL_STENCIL_ATTACHMENT.
 */
void texture_framebuffer_attachment::attachAs(GLuint target) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, target, textarget, tex->openGL_id, 0);
}

