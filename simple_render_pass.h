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
    directional_light d_light;///< Directional light used to render the scene.
    point_light p_light;///< The point light placed and rendered in the scene.
    glm::vec3 ambient_light_color;//< Color of light affecting all objects in the scene regardless of normals
    simple_render_pass_parameters(camera the_camera, directional_light d_light, point_light p_light, glm::vec3 ambient_light_color) : the_camera(the_camera), d_light(d_light), p_light(p_light), ambient_light_color(ambient_light_color) {
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
