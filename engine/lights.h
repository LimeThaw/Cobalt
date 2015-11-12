#ifndef LIGHT_H
#define LIGHT_H

#define  GLM_FORCE_RADIANS

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "util.h"
#include "node.h"

class light {
    public:
        light(glm::vec3 color = glm::vec3(255, 255, 255), float intensity = 1.0f);
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
        directional_light(glm::vec3 color = glm::vec3(255, 255, 255), float intensity = 1.0f, glm::vec3 direction = glm::vec3(0, 0, -1));
        void set_direction(glm::vec3 new_direction);
        const glm::vec3 &get_direction();

    private:
        glm::vec3 direction;
};

class point_light : public light, public node {
    public:
        point_light(glm::vec3 color = glm::vec3(255, 255, 255), float intensity = 1.0f, glm::vec3 position = glm::vec3(0, 0, 1), float radius = 10.0f);
        const glm::vec3 get_position();
        float get_radius();
    private:
        float radius;
};

#endif // LIGHT_H
