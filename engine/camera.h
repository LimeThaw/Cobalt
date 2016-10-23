#ifndef CAMERA_H
#define CAMERA_H

#define  GLM_FORCE_RADIANS

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "node.h"

/**
 *  The camera class holds view and projection matrices used for rendering.
 *  It is a direct descendant of the node class and as such inherits all its fields and mthods.
 *  It adds some functionalities which are only useful for camera nodes and a projection matrix
 *  crucial for rendering.
 */
class camera : public node {
    public:
        camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 look_point = glm::vec3(0.0f, 0.0f, -1.0f),
               glm::mat4 projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f));
        camera(glm::mat4 view, glm::mat4 projection);
        ~camera();
        void setup(glm::vec3 arg_pos, glm::vec3 arg_point);
        const glm::mat4 get_view() const;
        const glm::mat4 &get_projection() const;
        void set_projection(const glm::mat4 &matrix);

    private:
        glm::mat4 projection;
        bool first_person;
};

#endif // CAMERA_H
