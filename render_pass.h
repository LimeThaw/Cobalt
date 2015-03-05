#ifndef RENDER_PASS_INCLUDED
#define RENDER_PASS_INCLUDED

#include <vector>

#include "shader_manager.h"

template<typename scene_type>
class render_pass {
private:
    unsigned int shader_id;
public:
    render_pass(unsigned int shader_id) : shader_id(shader_id) {
    }
    
    void prepare_render() {
        shader_manager::get_instance().use_shader(shader_id);
    }
    
    
    virtual void render(scene_type &scene) = 0;
};

#endif