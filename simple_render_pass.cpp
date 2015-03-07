#include <algorithm>

#include "simple_render_pass.h"

simple_render_pass::simple_render_pass(shader_id new_shader_id, std::vector< unsigned int > render_material_ids) : render_pass< scene, simple_render_pass_parameters >(new_shader_id), render_material_ids(render_material_ids) {
}

simple_render_pass::simple_render_pass(shader_id new_shader_id, unsigned int render_material_id) : render_pass< scene, simple_render_pass_parameters >(new_shader_id), render_material_ids(std::vector<unsigned int> {render_material_id}) {
}

void simple_render_pass::render(scene &the_scene, simple_render_pass_parameters &additional_parameters) {
    camera the_camera = additional_parameters.the_camera;

    prepare_render();

    GLint active_shader_id;

    glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);
    GLuint view_id = glGetUniformLocation(active_shader_id, "view");
    glUniformMatrix4fv(view_id, 1, GL_FALSE, &the_camera.get_view()[0][0]);
    GLuint projection_id = glGetUniformLocation(active_shader_id, "projection");
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, &the_camera.get_projection()[0][0]);

    for(auto nodes : the_scene.enumerate_nodes()) {
        glm::mat4 node_matrix = nodes->get_node_matrix();
        glm::mat4 rotation_matrix = nodes->get_rotation_matrix();
        for(auto meshes : nodes->get_models()) {
            if(std::find(render_material_ids.begin(), render_material_ids.end(), meshes->get_material_id()) != render_material_ids.end()) {
                meshes->render(node_matrix, rotation_matrix);
            }
        }
    }
}
