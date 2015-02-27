#version 400
in vec2 uv;
in vec3 normal;
out vec3 color;
uniform sampler2D sampler;

void main(){
	vec3 light = vec3(-2, 0.5, 0.2);
	vec3 lightColor = vec3(0.9, 0.9, 0.9);
	color = texture(sampler, uv).rgb * lightColor * (clamp(dot(normalize(normal), normalize(light)), 0, 1)) + 0.1f * texture(sampler, uv).rgb;
}
