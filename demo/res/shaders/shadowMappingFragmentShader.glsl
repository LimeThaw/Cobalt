#version 130

#define NUM_DIRECTIONAL_LIGHTS 4

in vec2 uv;
in vec3 normal;
in vec3 view_position;
in vec3 shadow_map_space_position[NUM_DIRECTIONAL_LIGHTS];
out vec3 color;
uniform sampler2D color_map;
uniform sampler2D shadow_map[NUM_DIRECTIONAL_LIGHTS];

uniform vec3 ambient_light_color;

#if NUM_DIRECTIONAL_LIGHTS > 0
uniform vec3 directional_light_colors[NUM_DIRECTIONAL_LIGHTS];
uniform vec3 directional_light_directions[NUM_DIRECTIONAL_LIGHTS];
#endif

#define BIAS 0.005

void main(){
        vec3 texture_color = texture(color_map, uv).xyz;
        vec3 ambient_color = texture_color * ambient_light_color;
	color = ambient_color;
	vec3 local_normal = normalize(normal);
	
	#if NUM_DIRECTIONAL_LIGHTS > 0
        for(int i = 0; i < NUM_DIRECTIONAL_LIGHTS; ++i) {
            vec3 light_direction = directional_light_directions[i];
            vec3 light_color = directional_light_colors[i];
            if(texture2D(shadow_map[i], shadow_map_space_position[i].xy).x + BIAS > shadow_map_space_position[i].z) 
	      color += texture_color * light_color * clamp(dot(local_normal, light_direction), 0.0, 1.0);
        }
        #endif
        
        
}
