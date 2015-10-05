//
// Created by flocke on 05.10.15.
//

#include <GL/glew.h>
#include <sys/types.h>
#include "framebuffer.h"

std::shared_ptr<framebuffer> framebuffer::screen;

framebuffer::framebuffer(framebuffer::attachments color_attachments, framebuffer::optional_attachment depth_attachment,
                         framebuffer::optional_attachment stencil_attachment) : color_attachments(color_attachments),
                                                                                depth_attachment(depth_attachment),
                                                                                stencil_attachment(stencil_attachment) {
    glGenFramebuffers(1, &openGL_id);
    glBindFramebuffer(GL_FRAMEBUFFER, openGL_id);

    for(uint i = 0; i < color_attachments.size(); ++i) {
        color_attachments[i]->attachAs(GL_COLOR_ATTACHMENT0 + i);
    }

    if((bool) depth_attachment) {
        (*depth_attachment)->attachAs(GL_DEPTH_ATTACHMENT);
    }

    if((bool) stencil_attachment) {
        (*stencil_attachment)->attachAs(GL_STENCIL_ATTACHMENT);
    }

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("framebuffer error: " + std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
    }
}

framebuffer::framebuffer(GLuint openGL_id) : openGL_id(openGL_id) { }

framebuffer::~framebuffer() {
    glDeleteFramebuffers(1, &openGL_id);
}

void framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, openGL_id);
}

std::shared_ptr<framebuffer> framebuffer::get_screen() {
    if(!(bool) screen) {
        screen = std::make_shared<framebuffer>(0);
    }
    return screen;
}
