#include "std_render_pass.h"
#include "../mesh.h"
using namespace cs;

#define bias glm::mat4(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0)

const std::string shader_dir = "./engine/library/shader/";

std_render_pass::std_render_pass() :
		render_pass<scene, camera, std::vector<directional_light>, std::vector<point_light>>(std::make_shared<shader>(shader_dir + "std_shader.vertex",
			shader_dir + "std_shader.fragment", std::string("#version 130\n#define NUM_DIRECTIONAL_LIGHTS 1\n#define NUM_POINT_LIGHTS 1\n"))) {
	depth_shader = std::make_shared<shader>(shader_dir + "/depthOnlyVertexShader.glsl",
			shader_dir + "depthOnlyFragmentShader.glsl");
}

void std_render_pass::render(scene &a_scene, camera the_camera, std::vector<directional_light> d_lights, std::vector<point_light> p_lights) {

	if(d_lights.size() != num_d_lights || p_lights.size() != num_p_lights) {
		num_d_lights = d_lights.size();
		num_p_lights = p_lights.size();
		
		std_shader = std::make_shared<shader>(shader_dir + "std_shader.vertex",
			shader_dir + "std_shader.fragment",
			std::string("#version 130\n#define NUM_DIRECTIONAL_LIGHTS " + std::to_string(num_d_lights) +
			"\n#define NUM_POINT_LIGHTS " + std::to_string(num_p_lights) + "\n"));
		depth_shader = std::make_shared<shader>(shader_dir + "/depthOnlyVertexShader.glsl",
			shader_dir + "depthOnlyFragmentShader.glsl");
			
		// Generate shadow maps, framebuffers and bindings
		shadow_maps.clear();
		shadow_map_framebuffers.clear();
		shadow_map_bindings.clear();
		shadow_map_view_projections.clear();
		
		for(unsigned int i = 0; i < num_d_lights; ++i) {
			auto shadow_map = std::make_shared<texture2d>(
				512, 512, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_CLAMP, GL_CLAMP, GL_NEAREST, GL_NEAREST);
			auto shadow_map_attachment = std::make_shared<texture_framebuffer_attachment>(shadow_map);
			auto shadow_map_framebuffer = std::make_shared<framebuffer>(
				framebuffer::attachments(), framebuffer::optional_attachment(shadow_map_attachment));
			auto shadow_map_binding = material::texture_binding("shadow_map[" + std::to_string(i) + "]", shadow_map);
			
			shadow_map_view_projections.push_back(glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f)
				* glm::lookAt(d_lights[i].get_direction() * 10.0f, glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)));
			
			shadow_maps.push_back(shadow_map);
			shadow_map_framebuffers.push_back(shadow_map_framebuffer);
			shadow_map_bindings.push_back(shadow_map_binding);
		}
	}
	
	
	// Save viewport variables
	int viewport_variables[4];
	glGetIntegerv(GL_VIEWPORT, viewport_variables);
	
	// Render shadow maps
	set_shader(depth_shader);
	prepare_render();
	glViewport(0, 0, 512, 512);
    GLint active_shader_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);
	GLuint model_view_projection_id = glGetUniformLocation(active_shader_id, "model_view_projection_matrix");
	glDrawBuffer(GL_NONE);
	
	for(unsigned int i = 0; i < num_d_lights; ++i) {
		shadow_map_framebuffers[i]->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for(auto node : a_scene.enumerate_nodes()) {
			if(mesh *m = dynamic_cast<mesh*>(node)) {
				//if(m->is_shadow_caster()) {
					auto model_view_projection = shadow_map_view_projections[i] * m->get_node_matrix();
					glUniformMatrix4fv(model_view_projection_id, 1, GL_FALSE, &model_view_projection[0][0]);
					m->render_no_bind();
				//}
			}
		}
	}
	
	// Render scene
	set_shader(std_shader);
	prepare_render();
    framebuffer::get_screen()->bind();
    glViewport(viewport_variables[0], viewport_variables[1], viewport_variables[2], viewport_variables[3]);
    glDrawBuffer(GL_BACK);
	
    glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);
    glm::mat4 view_matrix = the_camera.get_view();

    GLuint view_id = glGetUniformLocation(active_shader_id, "view");
    glUniformMatrix4fv(view_id, 1, GL_FALSE, &view_matrix[0][0]);
    GLuint projection_id = glGetUniformLocation(active_shader_id, "projection");
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, &the_camera.get_projection()[0][0]);
    GLuint model_id = glGetUniformLocation(active_shader_id, "model");
    GLuint normal_id = glGetUniformLocation(active_shader_id, "normal_to_view_matrix");
    
    // Set shadow uniforms
    for(unsigned int i = 0; i < num_d_lights; ++i) {
    	GLuint mat_id = glGetUniformLocation(active_shader_id, ("shadow_map_view_projection_matrix[" + std::to_string(i) + "]").c_str());
    	glUniformMatrix4fv(mat_id, 1, GL_FALSE, &(bias * shadow_map_view_projections[i])[0][0]);
    }
    
    for(auto node : a_scene.enumerate_nodes()) {
        if(mesh *m = dynamic_cast<mesh*>(node)) {
            if(m->get_material()->is_standard()) {
            	for(unsigned int i = 0; i < shadow_maps.size(); ++i) {
            		m->get_material()->add_texture("shadow_map[" + std::to_string(i) + "]", shadow_maps[i]);
            	}
            }
        }
    }
    

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

