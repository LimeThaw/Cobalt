//
// Created by flocke on 05.10.15.
//

#include "texture_framebuffer_attachment.h"

texture_framebuffer_attachment::texture_framebuffer_attachment(std::shared_ptr<texture> tex) : tex(tex) {
}

texture_framebuffer_attachment::~texture_framebuffer_attachment() {
}

void texture_framebuffer_attachment::attachAs(GLuint target) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, target, GL_TEXTURE_2D, tex->openGL_id, 0);
}

