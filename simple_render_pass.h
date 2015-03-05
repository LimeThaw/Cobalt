#ifndef SIMPLE_RENDER_PASS_INCLUDED
#define SIMPLE_RENDER_PASS_INCLUDED

#include "render_pass.h"
#include "scene.h"
#include "node.h"

class simple_render_pass :
    public render_pass<scene> {
    private:
        std::vector<unsigned int> render_material_ids;
    public:
        simple_render_pass(unsigned int shader_id, std::vector<unsigned int> render_material_ids);
        
        void render(scene &the_scene) override;
};

#endif
