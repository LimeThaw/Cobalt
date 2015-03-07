#ifndef SIMPLE_RENDER_PASS_INCLUDED
#define SIMPLE_RENDER_PASS_INCLUDED

#include "render_pass.h"
#include "scene.h"
#include "node.h"
#include "camera.h"

struct simple_render_pass_parameters {
    camera the_camera;
    simple_render_pass_parameters(camera the_camera) : the_camera(the_camera) {
    }
};

class simple_render_pass :
    public render_pass<scene, simple_render_pass_parameters> {
    private:
        std::vector<unsigned int> render_material_ids;
    public:
        simple_render_pass(shader_id new_shader_id, std::vector<unsigned int> render_material_ids);
        simple_render_pass(shader_id new_shader_id, unsigned int render_material_id);
        void render(scene &the_scene, simple_render_pass_parameters &additional_parameters) override;
};

#endif
