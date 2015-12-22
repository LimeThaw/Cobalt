/**
Camera objects contain a view- and a projection matrix used for rendering.
*/

#ifndef CAMERA_H
#define CAMERA_H

#define  GLM_FORCE_RADIANS

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "node.h"

class camera : public node {
    public:
        camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 look_point = glm::vec3(0.0f, 0.0f, -1.0f),
               glm::mat4 projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f));
        camera(glm::mat4 view, glm::mat4 projection);
        ~camera();
        void setup(glm::vec3 arg_pos, glm::vec3 arg_point);///< Combines the functions set_position() and point_at()
        const glm::mat4 get_view() const;///< Returns the view matrix of the camera.
        const glm::mat4 &get_projection() const;///< Returns the projection matrix of the camera.

    private:
        glm::mat4 projection;
};

#endif // CAMERA_H
