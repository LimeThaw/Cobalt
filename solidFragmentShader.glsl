#version 400
in vec2 uv;
in vec3 normal;
uniform vec3 light_color;
out vec3 color;

void main(){
	vec3 light = vec3(-2, 1.5, 0.2);
	//vec3 lightColor = vec3(0.8f);
	vec3 lightColor = light_color;
	color = vec3(0.8f) * lightColor * (clamp(dot(normalize(normal), normalize(light)), 0, 1)) + vec3(0.1f);
}
