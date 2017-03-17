#include <algorithm>

#include "simple_render_pass.h"

simple_render_pass::simple_render_pass(shader_ptr new_shader, std::vector<std::shared_ptr<material>> render_materials)
    : render_pass<scene, camera, std::vector<directional_light>, std::vector<point_light>, glm::vec3, const framebuffer&>(
        new_shader), render_materials(render_materials) {
}

simple_render_pass::simple_render_pass(shader_ptr new_shader, std::shared_ptr<material> render_material)
    : render_pass<scene, camera, std::vector<directional_light>, std::vector<point_light>, glm::vec3, const framebuffer&>(
        new_shader), render_materials(std::vector<std::shared_ptr<material>> { render_material }) {
}

void simple_render_pass::render(scene &the_scene, camera the_camera, std::vector<directional_light> d_lights,
                                std::vector<point_light> p_lights, glm::vec3 ambient_light_color, const framebuffer &framebuffer) {
    prepare_render();
    glm::mat4 view_matrix = the_camera.get_view();

    framebuffer.bind();

    GLint active_shader_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);

    GLuint view_id = glGetUniformLocation(active_shader_id, "view");
    glUniformMatrix4fv(view_id, 1, GL_FALSE, &view_matrix[0][0]);
    GLuint projection_id = glGetUniformLocation(active_shader_id, "projection");
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, &the_camera.get_projection()[0][0]);

    for(unsigned int i = 0; i < d_lights.size(); ++i) {
        directional_light d_light = d_lights[i];
        glm::vec3 color_vec = d_light.get_color() * d_light.get_intensity();
        glm::vec3 direction_vec = glm::vec3((view_matrix * glm::vec4(d_light.get_direction(), 0.0f)));
        std::string is = std::to_string(i);
        glUniform3f(glGetUniformLocation(active_shader_id, ("directional_light_colors[" + is + "]").c_str()),
                    color_vec.x, color_vec.y, color_vec.z);
        glUniform3f(glGetUniformLocation(active_shader_id, ("directional_light_directions[" + is + "]").c_str()),
                    direction_vec.x, direction_vec.y, direction_vec.z);
    }

    for(unsigned int i = 0; i < p_lights.size(); ++i) {
        point_light p_light = p_lights[i];
        glm::vec3 point_color_vec = p_light.get_color() * p_light.get_intensity();
        glm::vec3 point_position_vec = glm::vec3((view_matrix * glm::vec4(p_light.get_position(), 1.0f)));
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

    for(auto node : the_scene.enumerate_nodes()) {
        if(mesh* m = dynamic_cast<mesh*>(node.get())) {
            if(m->get_material() && std::find(render_materials.begin(), render_materials.end(), m->get_material()) !=
                    render_materials.end()) {
                m->render();
            }
        }
    }
}
