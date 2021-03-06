#version 130

in vec3 vertex_position;
in vec2 vertex_UV;
in vec3 vertex_normal;
in vec3 vertex_tangent;

uniform mat3 normal_to_view_matrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv;
out vec3 normal;
out mat3 tangent_to_view_matrix;
out vec3 view_position;
out vec3 tangent_view_position;

void main() {
    vec4 world_position4 = model * vec4(vertex_position, 1.0);
    vec4 view_position4 = view * world_position4;
    view_position = view_position4.xyz;
	gl_Position = projection * view_position4;
	
	uv = vertex_UV;
	vec3 temp_normal = normal_to_view_matrix * vertex_normal;
	vec3 tangent = normal_to_view_matrix * vertex_tangent;
	tangent_to_view_matrix = mat3(tangent, cross(tangent, temp_normal), temp_normal);
	normal = temp_normal;
	tangent_view_position = transpose(tangent_to_view_matrix) * view_position;
}
