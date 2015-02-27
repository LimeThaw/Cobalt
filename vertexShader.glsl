#version 400
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;
uniform mat4 rotation;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec2 uv;
out vec3 normal;

void main(){
	mat4 MVP = projection * view * model;
	gl_Position = MVP * vec4(vertexPosition, 1.0);
	uv = vertexUV;
	normal = (rotation * vec4(vertexNormal, 1)).xyz;
}
