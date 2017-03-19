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

/**
 *  This class can hold various aspects of a frame rendered by OpenGL. Stores different channels in
 *  seperate "buffers" aka framebuffer_attachments.
 */
class framebuffer {
    public:
        typedef std::vector<std::shared_ptr<framebuffer_attachment>> attachments; ///< Convenience alias for array of shared attachments
        typedef std::experimental::optional<std::shared_ptr<framebuffer_attachment>> optional_attachment; ///< Convenience alias for optional shared attachment
    private:
        GLuint openGL_id; ///< The ID assigned to the buffer by OpenGL

        attachments color_attachments; ///< The color buffers (attachments)
        optional_attachment depth_attachment, stencil_attachment; ///< Other buffers: depth and stencil

        static std::shared_ptr<framebuffer> screen; ///< One static/global framebuffer for the window's screen
    public:
        explicit framebuffer(GLuint openGL_id);

        framebuffer(attachments color_attachments,
                    optional_attachment depth_attachment = optional_attachment(),
                    optional_attachment stencil_attachment = optional_attachment());

        framebuffer(std::shared_ptr<framebuffer_attachment> color_attachment,
                    optional_attachment depth_attachment = optional_attachment(),
                    optional_attachment stencil_attachment = optional_attachment());

        framebuffer(std::shared_ptr<framebuffer_attachment> color_attachment,
                    std::shared_ptr<framebuffer_attachment> depth_attachment);

        virtual ~framebuffer();

        void bind() const;

        static std::shared_ptr<framebuffer> get_screen();
};


#endif //COBALT_FRAMEBUFFER_H
