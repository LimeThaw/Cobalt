#version 130

in vec3 vertex_position;

uniform mat4 model_view_projection_matrix;

void main() {
  gl_Position = model_view_projection_matrix * vec4(vertex_position, 1.0);
}
