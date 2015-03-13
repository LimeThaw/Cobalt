#version 400
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_UV;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_tangent;
uniform mat3 normal_matrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec2 uv;
out vec3 normal;
out mat3 tangent_space_matrix;
out vec3 world_position;

void main(){
	mat4 MVP = projection * view * model;
	gl_Position = MVP * vec4(vertex_position, 1.0);
	
	uv = vertex_UV;
	vec3 temp_normal = normal_matrix * vertex_normal;
	vec3 tangent = (model * vec4(vertex_tangent, 0.0)).xyz;
	tangent_space_matrix = mat3(tangent, cross(tangent, temp_normal), temp_normal);
	normal = vertex_normal;
	world_position = (model * vec4(vertex_position, 1.0)).xyz;
}
