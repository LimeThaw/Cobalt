#version 400
in vec2 uv;
in vec3 normal;
out vec3 color;

void main(){
	vec3 light = vec3(-2, 1.5, 0.2);
	vec3 lightColor = vec3(0.8f);
	color = vec3(0.8f) * lightColor * (clamp(dot(normalize(normal), normalize(light)), 0, 1)) + vec3(0.1f);
}
