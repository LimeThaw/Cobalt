/**
Camera objects contain a view- and a projection matrix used for rendering.
*/

#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class camera {
    public:
        camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 look_at = glm::vec3(0.0f, 0.0f, -1.0f),
               glm::mat4 projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f));
        camera(glm::mat4 view, glm::mat4 projection);
        ~camera();
        void place(float arg_x, float arg_y, float arg_z);///< Places the camera at the specified point in world space.
        void place(glm::vec3 arg);
        void point_at(float arg_x, float arg_y, float arg_z);///< Rotates the camera to face the specified point in world space.
        void point_at(glm::vec3 arg);
        void setup(glm::vec3 arg_pos, glm::vec3 arg_point);///< Combines the functions place() and point_at()
        const glm::mat4 &get_view() const;///< Returns the view matrix of the camera.
        const glm::mat4 &get_projection() const;///< Returns the projection matrix of the camera.

    private:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 look_at;
        glm::mat4 view;
        glm::mat4 projection;
        bool first_person;
};

#endif // CAMERA_H
