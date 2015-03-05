#ifndef RENDER_PASS_INCLUDED
#define RENDER_PASS_INCLUDED

#include <vector>

template<typename scene_type>
class render_pass {
public:
    virtual void render(const scene_type &scene) = 0;
};

#endif