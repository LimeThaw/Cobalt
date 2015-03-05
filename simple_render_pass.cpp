#include <algorithm>

#include "simple_render_pass.h"

simple_render_pass::simple_render_pass(unsigned int shader_id, std::vector< unsigned int > render_material_ids): render_pass< scene >(shader_id), render_material_ids(render_material_ids) {

}


void simple_render_pass::render(scene &the_scene) {
    prepare_render();
    for(auto in : the_scene.enumerate_nodes()) {
        glm::mat4 node_matrix = in->get_node_matrix();
        glm::mat4 rotation_matrix = in->get_rotation_matrix();
        for(auto im : in->get_models()) {
            if(std::find(render_material_ids.begin(), render_material_ids.end(), im->get_material_id()) != render_material_ids.end()) {
                im->render(node_matrix, rotation_matrix);
            }
        }
    }
}
