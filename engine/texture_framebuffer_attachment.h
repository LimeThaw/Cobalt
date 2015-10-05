//
// Created by flocke on 05.10.15.
//

#ifndef COBALT_TEXTURE_FRAMEBUFFER_ATTACHMENT_H
#define COBALT_TEXTURE_FRAMEBUFFER_ATTACHMENT_H


#include <memory>
#include "framebuffer_attachment.h"
#include "texture.h"

class texture_framebuffer_attachment :
    public framebuffer_attachment {
    private:
        std::shared_ptr<texture> tex;
    public:
        texture_framebuffer_attachment(std::shared_ptr<texture> tex);

        ~texture_framebuffer_attachment();

        void attachAs(GLuint target) override;
};


#endif //COBALT_TEXTURE_FRAMEBUFFER_ATTACHMENT_H
