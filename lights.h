#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "util.h"

class light {
    public:
        light();
        light(glm::vec3 color, float intensity, const char* new_color_uniform);
        virtual ~light();
        void set_color(glm::vec3 new_color);
        const glm::vec3 &get_color();
        void set_intensity(float new_intensity);
        float get_intensity();
        void set_color_uniform(const char* new_color_uniform);
        void apply_color();

    private:
        glm::vec3 color;
        float intensity;
        const char* color_uniform;
};

class sun_light : public light {
    public:
        sun_light();
        sun_light(glm::vec3 color, float intensity, glm::vec3 direction, const char* new_color_uniform);
        void set_direction(glm::vec3 new_direction);
        const glm::vec3 &get_direction();
        void apply();

    private:
        glm::vec3 direction;
};

#endif // LIGHT_H
