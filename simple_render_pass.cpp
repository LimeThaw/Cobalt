#include <algorithm>

#include "simple_render_pass.h"

simple_render_pass::simple_render_pass(shader_id new_shader_id, std::vector< unsigned int > render_material_ids) : render_pass< scene, simple_render_pass_parameters >(new_shader_id), render_material_ids(render_material_ids) {
}

simple_render_pass::simple_render_pass(shader_id new_shader_id, unsigned int render_material_id) : render_pass< scene, simple_render_pass_parameters >(new_shader_id), render_material_ids(std::vector<unsigned int> {render_material_id}) {
}

void simple_render_pass::render(scene &the_scene, simple_render_pass_parameters &additional_parameters) {
    camera the_camera = additional_parameters.the_camera;
    directional_light d_light = additional_parameters.d_light;
    point_light p_light = additional_parameters.p_light;
    glm::vec3 ambient_light_color = additional_parameters.ambient_light_color;
    
    prepare_render();

    GLint active_shader_id;

    glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);
    GLuint view_id = glGetUniformLocation(active_shader_id, "view");
    glUniformMatrix4fv(view_id, 1, GL_FALSE, &the_camera.get_view()[0][0]);
    GLuint projection_id = glGetUniformLocation(active_shader_id, "projection");
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, &the_camera.get_projection()[0][0]);

    glm::vec3 color_vec = d_light.get_color() * d_light.get_intensity() / 255.0f;
    glm::vec3 direction_vec = d_light.get_direction();
    glUniform3f(glGetUniformLocation(active_shader_id, "directional_light_colors[0]"), color_vec.x, color_vec.y, color_vec.z);
    glUniform3f(glGetUniformLocation(active_shader_id, "directional_light_directions[0]"), direction_vec.x, direction_vec.y, direction_vec.z);

    glm::vec3 point_color_vec = p_light.get_color() * p_light.get_intensity() / 255.0f;
    glm::vec3 point_position_vec = p_light.get_position();
    glUniform3f(glGetUniformLocation(active_shader_id, "point_light_colors[0]"), point_color_vec.x, point_color_vec.y, point_color_vec.z);
    glUniform3f(glGetUniformLocation(active_shader_id, "point_light_positions[0]"), point_position_vec.x, point_position_vec.y, point_position_vec.z);
    
    
    glUniform3f(glGetUniformLocation(active_shader_id, "ambient_light_color"),  ambient_light_color.r, ambient_light_color.g, ambient_light_color.b);

    for(auto nodes : the_scene.enumerate_nodes()) {
        glm::mat4 node_matrix = nodes->get_node_matrix();
        for(auto meshes : nodes->get_models()) {
            if(std::find(render_material_ids.begin(), render_material_ids.end(), meshes->get_material_id()) != render_material_ids.end()) {
                meshes->render(node_matrix);
            }
        }
    }
}
