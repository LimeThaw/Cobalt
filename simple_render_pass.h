#ifndef SIMPLE_RENDER_PASS_INCLUDED
#define SIMPLE_RENDER_PASS_INCLUDED

#include "render_pass.h"
#include "scene.h"
#include "node.h"

class simple_render_pass :
    public render_pass<scene> {
    public:
        void render(const scene &the_scene) override;
};

#endif
