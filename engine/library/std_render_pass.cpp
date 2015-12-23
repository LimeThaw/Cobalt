#include "std_render_pass.h"
#include "../mesh.h"
using namespace cs;

std_render_pass::std_render_pass() :
    render_pass<scene, camera, std::vector<directional_light>, std::vector<point_light>>(std::make_shared<shader>("./engine/library/std_shader.vertex", "./engine/library/std_shader.fragment")) {
}

void std_render_pass::render(scene &a_scene, camera the_camera, std::vector<directional_light> d_lights, std::vector<point_light> p_lights) {
    prepare_render();
    GLint active_shader_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);
    glm::mat4 view_matrix = the_camera.get_view();

    GLuint view_id = glGetUniformLocation(active_shader_id, "view");
    glUniformMatrix4fv(view_id, 1, GL_FALSE, &view_matrix[0][0]);
    GLuint projection_id = glGetUniformLocation(active_shader_id, "projection");
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, &the_camera.get_projection()[0][0]);
    GLuint model_id = glGetUniformLocation(active_shader_id, "model");
    GLuint normal_id = glGetUniformLocation(active_shader_id, "normal_to_view_matrix");
    

    for(unsigned int i = 0; i < d_lights.size(); ++i) {
        directional_light d_light = d_lights[i];
        glm::vec3 color_vec = d_light.get_color() * clamp(d_light.get_intensity(), 0.0f, 1.0f);
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

    for(auto node : a_scene.enumerate_nodes()) {
        if(mesh *m = dynamic_cast<mesh*>(node)) {
            if(m->get_material()->is_standard()) {
                glm::mat4 render_model = m->get_node_matrix();
                glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(view_matrix * render_model)));
                glUniformMatrix4fv(model_id, 1, GL_FALSE, &render_model[0][0]);
		glUniformMatrix3fv(normal_id, 1, GL_FALSE, &normal_matrix[0][0]);
                m->render();
            }
        }
    }
}

