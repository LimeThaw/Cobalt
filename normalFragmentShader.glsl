#version 400
in vec2 uv;
in mat3 tangent_space_matrix;
out vec3 color;
uniform sampler2D color_map;
uniform sampler2D normal_map;

void main(){
	vec3 local_normal = (2.0 * texture(normal_map, uv).xyz) - vec3(1.0, 1.0, 1.0);
	local_normal = tangent_space_matrix * local_normal;
	vec3 light = vec3(-2, 0.5, 0.2);
	vec3 lightColor = vec3(0.9, 0.9, 0.9);
	vec3 texture_color = texture(color_map, uv).xyz;
	vec3 diffuse_color = texture_color * lightColor;
	vec3 diffuse = diffuse_color * clamp(dot(normalize(local_normal), normalize(light)), 0.0, 1.0);
	vec3 ambient = texture_color * 0.3f;
	color =  diffuse + ambient;
}
