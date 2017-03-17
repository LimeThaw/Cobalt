/**
Template used as  a basis for custom render passes.
*/

#ifndef RENDER_PASS_INCLUDED
#define RENDER_PASS_INCLUDED

#include <vector>

#include "shader.h"
#include "pointer_wrapper.h"

template<typename scene_type, typename... additional_render_parameter_types>
class render_pass {
private:
    shader_ptr render_pass_shader;
public:
    render_pass(shader_ptr render_pass_shader_id) : render_pass_shader(render_pass_shader_id) {
    }///< Constructor defining the shader to be used in the render pass.

	virtual ~render_pass() {

	}

    void prepare_render() {
        render_pass_shader->use();
    }///< Function binding the shader associated with the render pass.

	void set_shader(shader_ptr new_render_pass_shader) {
		render_pass_shader = new_render_pass_shader;
	}

    virtual void render(scene_type &scene, additional_render_parameter_types... additional_render_parameters) = 0;///< Render function that needs to be defined individually for every render pass.
};

template<typename scene_type, typename... additional_render_parameter_types>
using render_pass_ptr =
	pointer_wrapper<render_pass<scene_type, additional_render_parameter_types...>>;

#endif
