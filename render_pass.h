#ifndef RENDER_PASS_INCLUDED
#define RENDER_PASS_INCLUDED

#include <vector>

template<typename scene_render_parameters_container, typename node_type>
class render_pass {
public:
    virtual void render(const scene_render_parameters_container &scene_parameters, const std::vector<node_type> &nodes);
};

#endif