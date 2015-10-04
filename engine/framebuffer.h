//
// Created by flocke on 05.10.15.
//

#ifndef COBALT_FRAMEBUFFER_H
#define COBALT_FRAMEBUFFER_H


#include <GL/gl.h>
#include <memory>
#include <vector>
#include <experimental/optional>
#include "framebuffer_attachment.h"

class framebuffer {
    private:
        typedef std::vector<std::shared_ptr<framebuffer_attachment>> attachments;
        typedef std::experimental::optional<std::shared_ptr<framebuffer_attachment>> optional_attachment;

        GLuint openGL_id;

        attachments color_attachments;
        optional_attachment depth_attachment, stencil_attachment;

        static std::shared_ptr<framebuffer> screen;
    public:
        explicit framebuffer(GLuint openGL_id);
        framebuffer(attachments color_attachments,
                    optional_attachment depth_attachment = optional_attachment(),
                    optional_attachment stencil_attachment = optional_attachment());
        virtual ~framebuffer();

        void bind() const;

        static std::shared_ptr<framebuffer> get_screen();
};


#endif //COBALT_FRAMEBUFFER_H
