/**
\struct simple_render_pass_parameters
Struct used to pass custom parameters to the custom render pass.

\class simple_render_pass
Example class implementing render_pass used to render the example scene.
*/

#ifndef SIMPLE_RENDER_PASS_INCLUDED
#define SIMPLE_RENDER_PASS_INCLUDED

#include "cobalt.h"

struct simple_render_pass_parameters {
    camera the_camera;///< Camera used by the render pass to render the scene.
    directional_light light;///< Directional light used to render the scene.
    simple_render_pass_parameters(camera the_camera, directional_light light) : the_camera(the_camera), light(light) {
    }
};

class simple_render_pass :
    public render_pass<scene, simple_render_pass_parameters> {
    private:
        std::vector<unsigned int> render_material_ids;
    public:
        simple_render_pass(shader_id new_shader_id, std::vector<material_id> render_material_ids);///< Creating a render pass mith multiple materials.
        simple_render_pass(shader_id new_shader_id, material_id render_material_id);///< Creating a render pass with a single material.
        void render(scene &the_scene, simple_render_pass_parameters &additional_parameters) override;///< Renders all objects in the scene that have one of simple_render_pass's materials.
};

#endif
