#version 130

#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 3

in vec3 normal;
in vec3 eye_vector_world;
in vec2 uv;

uniform samplerCube environment;

out vec3 color;

void main() {
	color = texture(environment, normalize(normal)).xyz;
}
