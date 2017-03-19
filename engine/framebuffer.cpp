//
// Created by flocke on 05.10.15.
//

#include <GL/glew.h>
#include <sys/types.h>
#include "framebuffer.h"

std::shared_ptr<framebuffer> framebuffer::screen;

/**
 *  Creates a new framebuffer with the given attachments. First it stores them in the object, then
 *  it has OpenGL generate a new framebuffer and attach all the attachments. Finally checks if all
 *  went well and throws a runtime error if not. The second two arguments are optional (have a
 *  default value).
 *  @param	color_attachments	An array of framebuffer_attachment objects buffering the color
 *  								channels of the rendering output.
 *  @param	depth_attachment	If present buffers the depth information output by the shader.
 *  @param	stencil_attachment	If present buffers the stencil component of the rendered image.
 *  								For more information on stencil output see https://open.gl/depthstencils.
 */
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

/**
 *  Convenience overload: Just turns the one color attachment into an array of one for the first
 *  constructor. Only the first argument is mandatory, the others are optional.
 *  @param	color_attachment, depth_attachment, stencil_attachment	See first constructor.
 */
framebuffer::framebuffer(std::shared_ptr<framebuffer_attachment> color_attachment,
                         framebuffer::optional_attachment depth_attachment,
                         framebuffer::optional_attachment stencil_attachment) :
        framebuffer(framebuffer::attachments { color_attachment }, depth_attachment, stencil_attachment) {

}

/**
 *  Convenience overload: Turns the depth attachment into an optional attachment, doens't take a stencil
 *  attachment. None of the arguments are optional.
 *  @param	color_attachment, depth_attachment	See first constructor.
 */
framebuffer::framebuffer(std::shared_ptr<framebuffer_attachment> color_attachment,
                         std::shared_ptr<framebuffer_attachment> depth_attachment) :
        framebuffer(color_attachment, framebuffer::optional_attachment(depth_attachment)) {

}

/**
 *  Wait wut? This constructor takes the ID of an existing framebuffer object and makes it its own.
 *  It leaves all other members uninitialized.
 *  @param openGL_id	The ID assigned by OpenGL to an existing framebuffer object.
 */
framebuffer::framebuffer(GLuint openGL_id) : openGL_id(openGL_id) { }

/**
 *  Destructor. Makes sure that OpenGL deletes the associated framebuffer object.
 */
framebuffer::~framebuffer() {
    glDeleteFramebuffers(1, &openGL_id);
}

/**
 *  Makes the framebuffer active for OpenGL. This means that this is the buffer that OpenGL now
 *  renders to if any draw calls are executed.
 */
void framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, openGL_id);
}

/**
 *  Getter function for the static framebuffer object that is the screen.
 */
std::shared_ptr<framebuffer> framebuffer::get_screen() {
    if(!(bool) screen) {
        screen = std::make_shared<framebuffer>(0);
    }
    return screen;
}
