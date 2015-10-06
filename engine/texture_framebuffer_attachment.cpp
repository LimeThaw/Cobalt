//
// Created by flocke on 05.10.15.
//

#include <GL/glew.h>
#include "texture_framebuffer_attachment.h"

texture_framebuffer_attachment::texture_framebuffer_attachment(std::shared_ptr<texture> tex, GLenum textarget) : tex(
        tex), textarget(textarget) {
}

texture_framebuffer_attachment::~texture_framebuffer_attachment() {
}

void texture_framebuffer_attachment::attachAs(GLuint target) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, target, textarget, tex->openGL_id, 0);
}

