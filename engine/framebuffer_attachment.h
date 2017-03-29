//
// Created by flocke on 05.10.15.
//

#ifndef COBALT_FRAMEBUFFER_ATTACHMENT_H
#define COBALT_FRAMEBUFFER_ATTACHMENT_H


#include <GL/glew.h>

/**
 *  A component of a framebuffer. A framebuffer attachment can be attached to a framebuffer to store
 *  a component of a rendered image, like color, depth or stencil. see framebuffer for and idea on
 *  how and where to use it.
 */
class framebuffer_attachment {
        friend class framebuffer;
    public:
        virtual ~framebuffer_attachment(); ///< Virtual estructor
    private:
        virtual void attachAs(GLuint target) = 0; ///< Attaches the framebuffer attachment. Must be implemented in a child class.
};


#endif //COBALT_FRAMEBUFFER_ATTACHMENT_H
