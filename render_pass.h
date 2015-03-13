/**
Template used as  a basis for custom render passes.
*/

#ifndef RENDER_PASS_INCLUDED
#define RENDER_PASS_INCLUDED

#include <vector>

#include "shader_manager.h"

template<typename scene_type, typename... additional_render_parameter_types>
class render_pass {
private:
    shader_id render_pass_shader_id;
public:
    render_pass(shader_id render_pass_shader_id) : render_pass_shader_id(render_pass_shader_id) {
    }///< Constructor defining the shader to be used in the render pass.

    void prepare_render() {
        shader_manager::get_instance().use_shader(render_pass_shader_id);
    }///< Function binding the shader associated with the render pass.


    virtual void render(scene_type &scene, additional_render_parameter_types... additional_render_parameters) = 0;///< Render function that needs to be defined individually for every render pass.
};

#endif
