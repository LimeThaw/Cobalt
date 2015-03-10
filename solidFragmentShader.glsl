#version 400
in vec2 uv;
in vec3 normal;
in vec3 point_light_distance;
uniform vec3 light_color;
uniform vec3 light_direction;
uniform vec3 point_light_color;
out vec3 color;

void main(){
	//vec3 light = vec3(-2, 1.5, 0.2);
	vec3 light = light_direction;
	//vec3 lightColor = vec3(0.8f);
	vec3 lightColor = light_color;

	vec3 ambient_color = vec3(0.1f);
	vec3 directional_color = vec3(0.8f) * lightColor * (clamp(dot(normalize(normal), normalize(light)), 0, 1));
	vec3 point_color = vec3(0.8f) * point_light_color * (clamp(dot(normalize(normal), normalize(point_light_distance)), 0, 1)) / dot(point_light_distance, point_light_distance);
	color = ambient_color + directional_color + point_color;
}
