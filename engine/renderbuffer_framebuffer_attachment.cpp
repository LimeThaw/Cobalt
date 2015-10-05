//
// Created by flocke on 05.10.15.
//

#include <GL/glew.h>
#include "renderbuffer_framebuffer_attachment.h"

renderbuffer_framebuffer_attachment::renderbuffer_framebuffer_attachment(GLenum internalformat, GLsizei width,
                                                                         GLsizei height) {
    glGenRenderbuffers(1, &openGL_id);
    glBindRenderbuffer(GL_RENDERBUFFER, openGL_id);
    glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
}

renderbuffer_framebuffer_attachment::~renderbuffer_framebuffer_attachment() {
    glDeleteRenderbuffers(1, &openGL_id);
}

void renderbuffer_framebuffer_attachment::attachAs(GLuint target) {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, target, GL_RENDERBUFFER, openGL_id);
}
