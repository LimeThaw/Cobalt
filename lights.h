#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "util.h"

class light {
    public:
        light();
        light(glm::vec3 color, float intensity);
        virtual ~light();
        void set_color(glm::vec3 new_color);
        const glm::vec3 &get_color();
        void set_intensity(float new_intensity);
        float get_intensity();

    private:
        glm::vec3 color;
        float intensity;
};

class directional_light : public light {
    public:
        directional_light();
        directional_light(glm::vec3 color, float intensity, glm::vec3 direction);
        void set_direction(glm::vec3 new_direction);
        const glm::vec3 &get_direction();

    private:
        glm::vec3 direction;
};

#endif // LIGHT_H
