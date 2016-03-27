#include "std_scene.h"
#include <iostream>
using namespace cs;

std_scene::std_scene() : scene() {
	render_pass = std::make_shared<std_render_pass>();
}

void std_scene::set_camera(std::shared_ptr<camera> new_camera) {
	main_camera = new_camera;
}

void std_scene::add_point_light(std::shared_ptr<point_light> new_light) {
	p_lights.push_back(new_light);
}

void std_scene::add_directional_light(std::shared_ptr<directional_light> new_light) {
	d_lights.push_back(new_light);
}

void std_scene::set_skybox(std::string path) {
	if(skybox == NULL) {
		skybox = std::make_shared<mesh>("engine/library/skybox.obj");
		skybox_shader = std::make_shared<shader>("./engine/library/shader/skybox_shader.vertex", "engine/library/shader/skybox_shader.fragment");
	}
	auto skybox_mat = std::make_shared<material>();
	skybox_mat->add_texture("color_map", std::make_shared<texture2d>(
            texture_cache::get_instance().get_texture_from_filename(path)));
    skybox->set_material(skybox_mat);
}

void std_scene::render() {
	if(main_camera) {
		if(skybox) {
			skybox_shader->use();
			GLint active_shader_id;
			glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);
			
			GLuint model_id = glGetUniformLocation(active_shader_id, "model");
			glUniformMatrix4fv(model_id, 1, GL_FALSE, &skybox->get_node_matrix()[0][0]);
			GLuint view_id = glGetUniformLocation(active_shader_id, "view");
			glUniformMatrix4fv(view_id, 1, GL_FALSE, &main_camera->get_view()[0][0]);
			GLuint projection_id = glGetUniformLocation(active_shader_id, "projection");
			glUniformMatrix4fv(projection_id, 1, GL_FALSE, &main_camera->get_projection()[0][0]);
			
			glDepthMask(false);
			skybox->render();
			glDepthMask(true);
		}
		render_pass -> render(*this, *main_camera, d_lights, p_lights);
	} else {
		std::cerr << "! Tried to render scene without active camera\n";
	}
}
