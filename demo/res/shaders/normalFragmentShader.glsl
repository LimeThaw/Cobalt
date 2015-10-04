#version 400
#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 3
in vec2 uv;
in vec3 normal;
in mat3 view_to_tangent_matrix;
in vec3 view_position;
uniform sampler2D color_map;
uniform sampler2D normal_map;
#if NUM_DIRECTIONAL_LIGHTS > 0
uniform vec3 directional_light_colors[NUM_DIRECTIONAL_LIGHTS];
uniform vec3 directional_light_directions[NUM_DIRECTIONAL_LIGHTS];
#endif
#if NUM_POINT_LIGHTS > 0
uniform vec3 point_light_colors[NUM_POINT_LIGHTS];
uniform vec3 point_light_positions[NUM_POINT_LIGHTS];
uniform float point_light_radii_sq[NUM_POINT_LIGHTS];
#endif
uniform vec3 ambient_light_color;
out vec3 color;

void main(){
	vec3 local_normal = (2.0 * texture(normal_map, uv).xyz) - vec3(1.0);
	local_normal = normalize(view_to_tangent_matrix * local_normal);
	vec3 texture_color = texture(color_map, uv).xyz;
        vec3 ambient_color = texture_color * ambient_light_color;
        
        color = ambient_color;
                
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
            vec3 light_direction = point_light_positions[i] - view_position;
            float light_distance_sq = dot(light_direction, light_direction);
            vec3 light_color = point_light_colors[i];
            float radius_sq = point_light_radii_sq[i];
            float attenuation = clamp(1.0 - light_distance_sq / radius_sq, 0.0, 1.0);
            color += texture_color * light_color * clamp(dot(local_normal, normalize(light_direction)), 0.0, 1.0) * attenuation;
        }
        #endif
}
