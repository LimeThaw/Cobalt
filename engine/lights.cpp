#include "lights.h"

//light --------------------------------------------------------------------------------------------

/**
 *  Class constructor. Simply sets the color and intensity values. Both have default values.
 *  @param	new_color		The color of the light. Stored in RGB values between 0 and 1.
 *  @param	new_intensity	The intensity or brightness of the light. 1 is considered a "normal" value.
 */
light::light(glm::vec3 new_color, float new_intensity) {
    set_color(new_color);
    set_intensity(new_intensity);
}

/**
 *  Setter method for the color property.
 *  @param	new_color	The color of the light. Stored in RGB values between 0 and 1.
 */
void light::set_color(glm::vec3 new_color) {
    color.x = clamp(new_color.x, 0.0f, 1.0f);
    color.y = clamp(new_color.y, 0.0f, 1.0f);
    color.z = clamp(new_color.z, 0.0f, 1.0f);
}

/**
 *  Getter method for the color property.
 */
const glm::vec3 &light::get_color() {
    return color;
}

/**
 *  Setter method for the intensity property.
 *  @param	new_intensity	The intensity or brightness of the light. 1 is considered a "normal" value.
 */
void light::set_intensity(float new_intensity) {
    intensity = new_intensity;
}

/**
 *  Getter method for the intensity property.
 */
float light::get_intensity() {
    return intensity;
}

//directional_light --------------------------------------------------------------------------------

/**
 *  Class constructor. Adds initialization for the direction property to the light constructor.
 *  @param	color		The color of the light. Stored in RGB values between 0 and 1.
 *  @param	intensity	The intensity or brightness of the light. 1 is considered a "normal" value.
 *  @param	direction	The direction in which the light shines.
 *  @param	name		Name used to find the light within the scene.
 */
directional_light::directional_light(glm::vec3 color, float intensity, glm::vec3 direction,
		string name) : light(color, intensity), named(name) {
    set_direction(direction);
}

/**
 *  Passive destructor
 */
directional_light::~directional_light() {

}

/**
 *  Setter method for the direction property.
 *  @param	new_direction	The direction in which the light shines.
 */
void directional_light::set_direction(glm::vec3 new_direction) {
    if(glm::length(new_direction) != 0) {
        direction = glm::normalize(new_direction);
    } else {
        std::cerr << "! Can't assign a nullvector as direction for a sun light\n";
        return;
    }
}

/**
 *  Getter method for the direction property.
 */
const glm::vec3 &directional_light::get_direction() {
    return direction;
}

//point_light --------------------------------------------------------------------------------------

/**
 *  Class constructor. Adds initialization for position property to the light constructor.
 *  @param	color		The color of the light. Stored in RGB values between 0 and 1.
 *  @param	intensity	The intensity or brightness of the light. 1 is considered a "normal" value.
 *  @param	position	The position of the point light in 3D space.
 *  @param	radius		The illumination radius of the light. The intensity will quadratically decrease up to the radius. Beyond that the light won't be considered for rendering.
 *  @param	arg_name	Name used to find the light within the scene.
 */
point_light::point_light(glm::vec3 color, float intensity, glm::vec3 position, float radius, string arg_name) : light(color, intensity), node(arg_name), radius(radius) {
    place(position);
}

/**
 *  Getter method for the light's position.
 */
const glm::vec3 point_light::get_position() {
    return glm::vec3((node_matrix * glm::vec4(0, 0, 0, 1)));
}

/**
 *  Getter method for the radius property.
 */
float point_light::get_radius() {
    return radius;
}

