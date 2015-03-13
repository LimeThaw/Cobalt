/**
\struct simple_render_pass_parameters
Struct used to pass custom parameters to the custom render pass.

\class simple_render_pass
Example class implementing render_pass used to render the example scene.
*/

#ifndef SIMPLE_RENDER_PASS_INCLUDED
#define SIMPLE_RENDER_PASS_INCLUDED

#include "cobalt.h"

class simple_render_pass :
    public render_pass<scene, camera, std::vector<directional_light>, std::vector<point_light>, glm::vec3> {
    private:
        std::vector<unsigned int> render_material_ids;
    public:
        simple_render_pass(shader_id new_shader_id, std::vector<material_id> render_material_ids);///< Creating a render pass mith multiple materials.
        simple_render_pass(shader_id new_shader_id, material_id render_material_id);///< Creating a render pass with a single material.
        void render(scene &the_scene, camera the_camera, std::vector<directional_light> d_lights, std::vector<point_light> p_lights, glm::vec3 ambient_light_color) override;///< Renders all objects in the scene that have one of simple_render_pass's materials.
};

#endif
