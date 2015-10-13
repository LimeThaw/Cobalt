#version 130

in vec3 vertex_position;
in vec2 vertex_UV;
in vec3 vertex_normal;
in vec3 vertex_tangent;

uniform mat3 normal_matrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv;
out vec3 normal;
out mat3 view_to_tangent_matrix;
out vec3 world_position;
out vec3 eye_vector_world;

void main(){
	mat4 MVP = projection * view * model;
	gl_Position = MVP * vec4(vertex_position, 1.0);
	
	uv = vertex_UV;
	vec3 temp_normal = (transpose(model) * vec4(vertex_normal, 1.0)).xyz;
	vec3 tangent = (model * vec4(vertex_tangent, 0.0)).xyz;
	view_to_tangent_matrix = mat3(tangent, cross(tangent, temp_normal), temp_normal);
	normal = temp_normal;
	world_position = (model * vec4(vertex_position, 1.0)).xyz;
	vec3 camera_world_position = (view * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	eye_vector_world = world_position - camera_world_position;
}
