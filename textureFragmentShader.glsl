#version 400
in vec2 uv;
in vec3 normal;
out vec3 color;
uniform sampler2D color_map;
uniform sampler2D normal_map;

void main(){
	vec3 light = vec3(-2, 0.5, 0.2);
	vec3 lightColor = vec3(0.9, 0.9, 0.9);
	color = texture(color_map, uv).rgb * lightColor * (clamp(dot(normalize(normal), normalize(light)), 0, 1)) + 0.1f * texture(color_map, uv).rgb;
}
