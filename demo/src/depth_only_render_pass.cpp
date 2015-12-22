#include "depth_only_render_pass.h"

depth_only_render_pass::depth_only_render_pass(std::shared_ptr<shader> the_shader) : render_pass<scene, const glm::mat4&,
  const framebuffer &>(the_shader) {
}

void depth_only_render_pass::render(scene& the_scene, const glm::mat4& view_projection_matrix, const framebuffer &framebuffer) {
  prepare_render();
  framebuffer.bind();

  GLint active_shader_id;
  glGetIntegerv(GL_CURRENT_PROGRAM, &active_shader_id);
  GLuint model_view_projection_id = glGetUniformLocation(active_shader_id, "model_view_projection_matrix");
  
  for(auto node : the_scene.enumerate_nodes()) {
      if(mesh *m = dynamic_cast<mesh*>(node)) {
	auto model_view_projection_matrix = view_projection_matrix * m->get_node_matrix();
	glUniformMatrix4fv(model_view_projection_id, 1, GL_FALSE, &model_view_projection_matrix[0][0]);
	m->render_no_bind_matrices();
      }
  }
}
