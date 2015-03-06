#ifndef SIMPLE_RENDER_PASS_INCLUDED
#define SIMPLE_RENDER_PASS_INCLUDED

#include "render_pass.h"
#include "scene.h"
#include "node.h"
#include "camera.h"

struct simple_render_pass_additional_parameters {
    camera the_camera;
    simple_render_pass_additional_parameters(camera the_camera) : the_camera(the_camera) {
    }
};

class simple_render_pass :
    public render_pass<scene, simple_render_pass_additional_parameters> {
    private:
        std::vector<unsigned int> render_material_ids;
    public:
        simple_render_pass(unsigned int shader_id, std::vector<unsigned int> render_material_ids);
        
        void render(scene &the_scene, simple_render_pass_additional_parameters &additional_parameters) override;
};

#endif
