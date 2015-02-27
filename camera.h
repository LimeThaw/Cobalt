#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class camera{
    public:
        camera();
        ~camera();
        void place(float arg_x, float arg_y, float arg_z);
        void place(glm::vec3 arg);
        void point_at(float arg_x, float arg_y, float arg_z);
        void point_at(glm::vec3 arg);
        void setup(glm::vec3 arg_pos, glm::vec3 arg_point);
        void set_first_person(bool arg);
        void update_motion();
        void update();

    private:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 look_at;
        glm::mat4 view;
        glm::mat4 projection;
        bool first_person;
};

#endif // CAMERA_H
