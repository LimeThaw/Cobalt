//
// Created by flocke on 05.10.15.
//

#ifndef COBALT_FRAMEBUFFER_ATTACHMENT_H
#define COBALT_FRAMEBUFFER_ATTACHMENT_H


#include <GL/gl.h>

class framebuffer_attachment {
        friend class framebuffer;

    private:
        virtual void attachAs(GLuint target) = 0;
};


#endif //COBALT_FRAMEBUFFER_ATTACHMENT_H
