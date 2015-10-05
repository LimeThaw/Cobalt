//
// Created by flocke on 05.10.15.
//

#ifndef COBALT_TEXTURE_RENDERBUFFER_ATTACHMENT_H
#define COBALT_TEXTURE_RENDERBUFFER_ATTACHMENT_H


#include "framebuffer_attachment.h"

class renderbuffer_framebuffer_attachment :
        public framebuffer_attachment {
    private:
        GLuint openGL_id;

    public:
        renderbuffer_framebuffer_attachment(GLenum internalformat, GLsizei width, GLsizei height);

        ~renderbuffer_framebuffer_attachment();

        void attachAs(GLuint target) override;
};


#endif //COBALT_TEXTURE_RENDERBUFFER_ATTACHMENT_H
