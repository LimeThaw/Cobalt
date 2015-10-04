#include <algorithm>

#include "simple_render_pass.h"

simple_render_pass::simple_render_pass(shader_id new_shader_id, std::vector<unsigned int> render_material_ids)
        : render_pass<scene, camera, std::vector<directional_light>, std::vector<point_light>, glm::vec3, const framebuffer&>(
        new_shader_id), render_material_ids(render_material_ids) {
}

simple_render_pass::simple_render_pass(shader_id new_shader_id, unsigned int render_material_id)
        : render_pass<scene, camera, std::vector<directional_light>, std::vector<point_light>, glm::vec3, const framebuffer&>(
        new_shader_id), render_material_ids(std::vector<unsigned int> { render_material_id }) {
}

void simple_render_pass::render(scene &the_scene, camera the_camera, std::vector<directional_light> d_lights,
                                std::vector<point_light> p_lights, glm::vec3 ambient_light_color, const framebuffer &framebuffer) {
    prepare_render();

    framebuffer.bind();

    GLint active_shader_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);

    GLuint view_id = glGetUniformLocation(active_shader_id, "view");
    glUniformMatrix4fv(view_id, 1, GL_FALSE, &the_camera.get_view()[0][0]);
    GLuint projection_id = glGetUniformLocation(active_shader_id, "projection");
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, &the_camera.get_projection()[0][0]);

    for(unsigned int i = 0; i < d_lights.size(); ++i) {
        directional_light d_light = d_lights[i];
        glm::vec3 color_vec = d_light.get_color() * d_light.get_intensity() / 255.0f;
        glm::vec3 direction_vec = d_light.get_direction();
        std::string is = std::to_string(i);
        glUniform3f(glGetUniformLocation(active_shader_id, ("directional_light_colors[" + is + "]").c_str()),
                    color_vec.x, color_vec.y, color_vec.z);
        glUniform3f(glGetUniformLocation(active_shader_id, ("directional_light_directions[" + is + "]").c_str()),
                    direction_vec.x, direction_vec.y, direction_vec.z);
    }

    for(unsigned int i = 0; i < p_lights.size(); ++i) {
        point_light p_light = p_lights[i];
        glm::vec3 point_color_vec = p_light.get_color() * p_light.get_intensity() / 255.0f;
        glm::vec3 point_position_vec = p_light.get_position();
        std::string is = std::to_string(i);
        glUniform3f(glGetUniformLocation(active_shader_id, ("point_light_colors[" + is + "]").c_str()),
                    point_color_vec.x, point_color_vec.y, point_color_vec.z);
        glUniform3f(glGetUniformLocation(active_shader_id, ("point_light_positions[" + is + "]").c_str()),
                    point_position_vec.x, point_position_vec.y, point_position_vec.z);
        float r = p_light.get_radius();
        glUniform1f(glGetUniformLocation(active_shader_id, ("point_light_radii_sq[" + is + "]").c_str()), r * r);
    }

    glUniform3f(glGetUniformLocation(active_shader_id, "ambient_light_color"), ambient_light_color.r,
                ambient_light_color.g, ambient_light_color.b);

    for(auto nodes : the_scene.enumerate_nodes()) {
        glm::mat4 node_matrix = nodes->get_node_matrix();
        for(auto meshes : nodes->get_models()) {
            if(std::find(render_material_ids.begin(), render_material_ids.end(), meshes->get_material_id()) !=
               render_material_ids.end()) {
                meshes->render(node_matrix);
            }
        }
    }
}
