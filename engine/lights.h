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
    	directional_light();
        directional_light(glm::vec3 color = glm::vec3(255, 255, 255), float intensity = 1.0f,
			glm::vec3 direction = glm::vec3(0, 0, -1), string arg_name = "");
        ~directional_light();
        void set_direction(glm::vec3 new_direction);
        const glm::vec3 &get_direction();
        string set_name(const string arg_name);
        string get_name();

    private:
        glm::vec3 direction;
        string name;
};

class point_light : public light, public node {
    public:
        point_light(glm::vec3 color = glm::vec3(255, 255, 255), float intensity = 1.0f, glm::vec3 position = glm::vec3(0, 0, 0), float radius = 10.0f, string arg_name = "");
        const glm::vec3 get_position();
        float get_radius();
    private:
        float radius;
};

#endif // LIGHT_H
