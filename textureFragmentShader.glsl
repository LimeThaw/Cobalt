#version 400
#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 1
in vec2 uv;
in vec3 normal;
in vec3 world_position;
out vec3 color;
uniform sampler2D color_map;

#if NUM_DIRECTIONAL_LIGHTS > 0
uniform vec3 directional_light_colors[NUM_DIRECTIONAL_LIGHTS];
uniform vec3 directional_light_directions[NUM_DIRECTIONAL_LIGHTS];
#endif
#if NUM_POINT_LIGHTS > 0
uniform vec3 point_light_colors[NUM_POINT_LIGHTS];
uniform vec3 point_light_positions[NUM_POINT_LIGHTS];
#endif
uniform vec3 ambient_light_color;

void main(){
        vec3 texture_color = texture(color_map, uv).xyz;
        vec3 ambient_color = texture_color * ambient_light_color;
        
        color = ambient_color;
        vec3 local_normal = normalize(normal);
        
        #if NUM_DIRECTIONAL_LIGHTS > 0
        for(int i = 0; i < NUM_DIRECTIONAL_LIGHTS; ++i) {
            vec3 light_direction = directional_light_directions[i];
            vec3 light_color = directional_light_colors[i];
            color += texture_color * light_color * clamp(dot(local_normal, light_direction), 0.0, 1.0);
        }
        #endif
        
        #if NUM_POINT_LIGHTS > 0
        for(int i = 0; i < NUM_POINT_LIGHTS; ++i) {
            vec3 light_position = point_light_positions[i];
            vec3 light_direction = point_light_positions[i] - world_position;
            float light_distance_sq = dot(light_direction, light_direction);
            vec3 light_color = point_light_colors[i];
            vec3 point =  texture_color * light_color * clamp(dot(local_normal, normalize(light_direction)), 0.0, 1.0) / light_distance_sq;
        }
        #endif
}