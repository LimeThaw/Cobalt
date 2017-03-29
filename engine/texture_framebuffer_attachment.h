//
// Created by flocke on 05.10.15.
//

#ifndef COBALT_TEXTURE_FRAMEBUFFER_ATTACHMENT_H
#define COBALT_TEXTURE_FRAMEBUFFER_ATTACHMENT_H


#include <memory>
#include "framebuffer_attachment.h"
#include "texture.h"

/**
 *  framebuffer_attachment implementation meant for rendering to textures. If you a have complex
 *  render pipeline and need to render something to texture to use it in a later pass, or just want
 *  to save screenshots, then this is the right implementation for you.
 */
class texture_framebuffer_attachment :
    public framebuffer_attachment {
    private:
        std::shared_ptr<texture> tex;
        GLenum textarget;
    public:
        texture_framebuffer_attachment(std::shared_ptr<texture> tex, GLenum textarget = GL_TEXTURE_2D);

        ~texture_framebuffer_attachment();

        void attachAs(GLuint target) override;
};


#endif //COBALT_TEXTURE_FRAMEBUFFER_ATTACHMENT_H
