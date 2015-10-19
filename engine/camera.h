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
#include "node.h"

class camera {
    public:
        camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 look_at = glm::vec3(0.0f, 0.0f, -1.0f),
               glm::mat4 projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f));
        camera(glm::mat4 view, glm::mat4 projection);
        ~camera();
        void set_position(float arg_x, float arg_y, float arg_z);///< Places the camera at the specified point in world space. If the camera has a parent node the position is set relative to that node.
        void set_position(glm::vec3 arg);;///< See member set_position(float arg_x, float arg_y, float arg_z).
        void point_at(float arg_x, float arg_y, float arg_z);///< Rotates the camera to face the specified point in world space. If the camera has a parent node the target point is set relative to that node.
        void point_at(glm::vec3 arg);///< See member point_at(float arg_x, float arg_y, float arg_z).
        void setup(glm::vec3 arg_pos, glm::vec3 arg_point);///< Combines the functions set_position() and point_at()
        const glm::mat4 get_view() const;///< Returns the view matrix of the camera.
        const glm::mat4 &get_projection() const;///< Returns the projection matrix of the camera.
        void set_parent(node *new_parent);///< Assigns a parent node to the camera. All other coordinates will be set relative to the parent node.

    private:
    	node *parent;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 look_at;
        glm::mat4 view;
        glm::mat4 projection;
        bool first_person;
};

#endif // CAMERA_H
