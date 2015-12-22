#ifndef DEPTH_ONLY_RENDER_PASS_INCLUDED
#define DEPTH_ONLY_RENDER_PASS_INCLUDED

#include "cobalt.h"
#include "framebuffer.h"

class depth_only_render_pass :
        public render_pass<scene, const glm::mat4&, const framebuffer &> {
    public:
        depth_only_render_pass(std::shared_ptr<shader> the_shader);

        void render(scene &the_scene, const glm::mat4 &view_projection_matrix, const framebuffer &framebuffer) override;
};

#endif
