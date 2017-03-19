#ifndef LIGHT_H
#define LIGHT_H

#define  GLM_FORCE_RADIANS

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "util.h"
#include "node.h"
#include "pointer_wrapper.h"

/**
 *  General light class. Fixes attributes that every proper light should have (color and intensity).
 *  Also abstracts most functionality. This class itself is not useful for most cases, except maybe
 *  ambient light or something.
 */
class light {
    public:
        light(glm::vec3 color = glm::vec3(255, 255, 255), float intensity = 1.0f);
        void set_color(glm::vec3 new_color);
        const glm::vec3 &get_color();
        void set_intensity(float new_intensity);
        float get_intensity();

    private:
        glm::vec3 color; ///< The color of the light. Stored in RGB values between 0 and 1.
        float intensity; ///< The intensity or brightness of the light. 1 is considered a "normal" value.
};

/**
 *  A uniform, constant intensity directional light. Shines in the same direction everywhere.
 *  Useful for sun light.
 */
class directional_light : public light, public named {
    public:
        directional_light(glm::vec3 color = glm::vec3(255, 255, 255), float intensity = 1.0f,
			glm::vec3 direction = glm::vec3(0, 0, -1), string name = "");
        ~directional_light();
        void set_direction(glm::vec3 new_direction);
        const glm::vec3 &get_direction();

    private:
        glm::vec3 direction; ///< The direction in which the light shines.
};

/**
 *  A positional light without volume. The light will shine brightest in its position and decrease
 *  in brightness with the square of distance up to a certain radius, beyon which it has no effect.
 *  Useful for small indoor lights like light bulbs or candles.
 */
class point_light : public light, public node {
    public:
        point_light(glm::vec3 color = glm::vec3(255, 255, 255), float intensity = 1.0f, glm::vec3 position = glm::vec3(0, 0, 0), float radius = 10.0f, string arg_name = "");
        const glm::vec3 get_position();
        float get_radius();
    private:
        float radius; ///< The illumination radius of the light. The intensity will quadratically decrease up to the radius. Beyond that the light won't be considered for rendering.
};

typedef pointer_wrapper<directional_light> directional_light_ptr; ///< Convenience alias
typedef pointer_wrapper<point_light> point_light_ptr; ///< Convenience alias

#endif // LIGHT_H
