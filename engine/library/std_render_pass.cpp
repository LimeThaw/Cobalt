#include "std_render_pass.h"
#include "../mesh.h"
using namespace cs;

#define bias glm::mat4(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 1.0)

const std::string shader_dir = "./engine/library/shader/";
const int shadow_map_resolution = 2048;
const glm::vec3 cubemap_directions[6] = {
	glm::vec3(1, 0, 0),
	glm::vec3(-1, 0, 0),
	glm::vec3(0, -1, 0),
	glm::vec3(0, 1, 0),
	glm::vec3(0, 0, 1),
	glm::vec3(0, 0, -1),
};
const glm::vec3 cubemap_ups[6] = {
	glm::vec3(0, 1, 0),
	glm::vec3(0, 1, 0),
	glm::vec3(0, 0, 1),
	glm::vec3(0, 0, -1),
	glm::vec3(0, 1, 0),
	glm::vec3(0, 1, 0),
};

std_render_pass::std_render_pass() :
		render_pass<scene, camera, std::vector<std::shared_ptr<directional_light>>, std::vector<std::shared_ptr<point_light>>>(std::make_shared<shader>(shader_dir + "std_shader.vertex",
			shader_dir + "std_shader.fragment", std::string("#version 130\n#define NUM_DIRECTIONAL_LIGHTS 1\n#define NUM_POINT_LIGHTS 1\n"))) {
	depth_shader = std::make_shared<shader>(shader_dir + "/depthOnlyVertexShader.glsl",
			shader_dir + "depthOnlyFragmentShader.glsl");
	point_light_shader = std::make_shared<shader>(shader_dir + "/point_light_shadow_shader.vertex",
			shader_dir + "depthOnlyFragmentShader.glsl");
}

void std_render_pass::render(scene &a_scene, camera the_camera, std::vector<std::shared_ptr<directional_light>> d_lights, std::vector<std::shared_ptr<point_light>> p_lights) {

	// Reload shaders and shadowmaps in case number of lights has changed
	if(d_lights.size() != num_d_lights || p_lights.size() != num_p_lights) {
		num_d_lights = d_lights.size();
		num_p_lights = p_lights.size();
		
		// Reload shaders
		std_shader = std::make_shared<shader>(shader_dir + "std_shader.vertex",
			shader_dir + "std_shader.fragment",
			std::string("#version 130\n#define NUM_DIRECTIONAL_LIGHTS " + std::to_string(num_d_lights) +
			"\n#define NUM_POINT_LIGHTS " + std::to_string(num_p_lights) + "\n"));
			
		// Generate directional shadow maps and framebuffers
		directional_shadow_maps.clear();
		directional_shadow_map_framebuffers.clear();
		directional_shadow_map_view_projections.clear();
		
		for(unsigned int i = 0; i < num_d_lights; ++i) {
			auto shadow_map = std::make_shared<texture2d>(
				shadow_map_resolution, shadow_map_resolution, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_CLAMP, GL_CLAMP, GL_NEAREST, GL_NEAREST);
			auto shadow_map_attachment = std::make_shared<texture_framebuffer_attachment>(shadow_map);
			auto shadow_map_framebuffer = std::make_shared<framebuffer>(
				framebuffer::attachments(), framebuffer::optional_attachment(shadow_map_attachment));
			
			directional_shadow_map_view_projections.push_back(get_light_matrix(d_lights[i]->get_direction(), a_scene));
			
			directional_shadow_maps.push_back(shadow_map);
			directional_shadow_map_framebuffers.push_back(shadow_map_framebuffer);
		}
		
		// Generate point-light shadow maps and framebuffers
		point_shadow_maps.clear();
		point_shadow_map_framebuffers.clear();
		for(unsigned int i = 0; i < num_p_lights; ++i) {
			auto shadow_map = std::make_shared<cubemap>(shadow_map_resolution/4.0f, shadow_map_resolution/4.0f, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_CLAMP, GL_CLAMP, GL_CLAMP, GL_NEAREST, GL_NEAREST);
			point_shadow_maps.push_back(shadow_map);
			
			for(GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X; face <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; ++face) {
				auto shadow_map_attachment = std::make_shared<texture_framebuffer_attachment>(shadow_map, face);
				point_shadow_map_framebuffers.push_back(std::make_shared<framebuffer>(framebuffer::attachments(), framebuffer::optional_attachment(shadow_map_attachment)));
			}
		}
	} else {
		// Update light matrices
		for(unsigned int i = 0; i < num_d_lights; ++i) {
			directional_shadow_map_view_projections[i] = get_light_matrix(d_lights[i]->get_direction(), a_scene);
		}
	}
	
	// Save viewport variables
	int viewport_variables[4];
	glGetIntegerv(GL_VIEWPORT, viewport_variables);
	
	// Render directional shadow maps
	set_shader(depth_shader);
	prepare_render();
	glViewport(0, 0, shadow_map_resolution, shadow_map_resolution);
    GLint active_shader_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);
	GLuint model_view_projection_id = glGetUniformLocation(active_shader_id, "model_view_projection_matrix");
	glDrawBuffer(GL_NONE);
	
	for(unsigned int i = 0; i < num_d_lights; ++i) {
		directional_shadow_map_framebuffers[i]->bind();
        glClear(GL_DEPTH_BUFFER_BIT);
		for(auto node : a_scene.enumerate_nodes()) {
			if(mesh *m = dynamic_cast<mesh*>(node)) {
				if(m->is_shadow_caster()) {
					auto model_view_projection = directional_shadow_map_view_projections[i] * m->get_node_matrix();
					glUniformMatrix4fv(model_view_projection_id, 1, GL_FALSE, &model_view_projection[0][0]);
					m->render_no_bind();
				}
			}
		}
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	//Render point light shadow maps
	glViewport(0, 0, shadow_map_resolution/4, shadow_map_resolution/4);
	assert(point_shadow_map_framebuffers.size() == 6 * num_p_lights);
	for(unsigned int i = 0; i < num_p_lights; ++i) {
		glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, p_lights[i]->get_radius());
		for(unsigned int j = 0; j < 6; ++j) {
			point_shadow_map_framebuffers[i * 6 + j]->bind();
		    glClear(GL_DEPTH_BUFFER_BIT);
		    glm::mat4 view_projection = projection * glm::lookAt(p_lights[i]->get_position(), p_lights[i]->get_position() + cubemap_directions[j], cubemap_ups[j]);
		    glUniform3f(glGetUniformLocation(active_shader_id, "light_position"), p_lights[i]->get_position().x, p_lights[i]->get_position().y, p_lights[i]->get_position().z);
			glUniform1f(glGetUniformLocation(active_shader_id, "radius"), p_lights[i]->get_radius());
			for(auto node : a_scene.enumerate_nodes()) {
				if(mesh *m = dynamic_cast<mesh*>(node)) {
					if(m->is_shadow_caster()) {
						auto model_view_projection = view_projection * m->get_node_matrix();
						glUniformMatrix4fv(model_view_projection_id, 1, GL_FALSE, &model_view_projection[0][0]);
						m->render_no_bind();
					}
				}
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
    glm::mat4 inverse_view_matrix = glm::inverse(view_matrix);

    GLuint view_id = glGetUniformLocation(active_shader_id, "view");
    glUniformMatrix4fv(view_id, 1, GL_FALSE, &view_matrix[0][0]);
    GLuint inverse_view_id = glGetUniformLocation(active_shader_id, "inverse_view");
    glUniformMatrix4fv(inverse_view_id, 1, GL_FALSE, &inverse_view_matrix[0][0]);
    GLuint projection_id = glGetUniformLocation(active_shader_id, "projection");
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, &the_camera.get_projection()[0][0]);
    GLuint model_id = glGetUniformLocation(active_shader_id, "model");
    GLuint normal_id = glGetUniformLocation(active_shader_id, "normal_to_view_matrix");
    
    // Set shadow uniforms
    for(unsigned int i = 0; i < num_d_lights; ++i) {
    	GLuint mat_id = glGetUniformLocation(active_shader_id, ("shadow_map_view_projection_matrix[" + std::to_string(i) + "]").c_str());
    	glUniformMatrix4fv(mat_id, 1, GL_FALSE, &(bias * directional_shadow_map_view_projections[i])[0][0]);
    }
    
    for(auto node : a_scene.enumerate_nodes()) {
        if(mesh *m = dynamic_cast<mesh*>(node)) {
            if(m->get_material()->is_standard()) {
            	for(unsigned int i = 0; i < directional_shadow_maps.size(); ++i) {
            		m->get_material()->add_texture("directional_shadow_map[" + std::to_string(i) + "]", directional_shadow_maps[i]);
            	}
            	for(unsigned int i = 0; i < point_shadow_maps.size(); ++i) {
            		m->get_material()->add_texture("point_shadow_map[" + std::to_string(i) + "]", point_shadow_maps[i]);
            	}
            }
        }
    }
    
	// Set directional light uniforms
    for(unsigned int i = 0; i < d_lights.size(); ++i) {
        directional_light d_light = *d_lights[i];
        glm::vec3 color_vec = d_light.get_color() * clamp(d_light.get_intensity(), 0.0f, 1.0f);
        glm::vec3 direction_vec = glm::vec3((view_matrix * glm::vec4(d_light.get_direction(), 0.0f)));
        std::string is = std::to_string(i);
        glUniform3f(glGetUniformLocation(active_shader_id, ("directional_light_colors[" + is + "]").c_str()),
                    color_vec.x, color_vec.y, color_vec.z);
        glUniform3f(glGetUniformLocation(active_shader_id, ("directional_light_directions[" + is + "]").c_str()),
                    direction_vec.x, direction_vec.y, direction_vec.z);
    }

	// Set point light uniforms
    for(unsigned int i = 0; i < p_lights.size(); ++i) {
        point_light p_light = *p_lights[i];
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

	// Render individual meshes
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

glm::mat4 std_render_pass::get_light_matrix(glm::vec3 direction, scene& a_scene) {
	bounding_box light_box = bounding_box(direction);
	for(auto node : a_scene.enumerate_nodes()) {
		if(mesh *m = dynamic_cast<mesh*>(node)) {
			if(m->is_shadow_caster()) {
				light_box.add_bounding_box(m->get_node_matrix() * m->get_bounding_box());
			}
	
		}
	}
	return light_box.get_light_matrix();
}
