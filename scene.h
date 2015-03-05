//Class describing the whole scene to be rendered
//parent_node should be the topmost node of the scene graph
//TODO: add skybox, lights, etc

#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "node.h"

class scene {
    public:
        scene();
        ~scene();
        void append_node(const char *file_path);
        void append_node(node *new_child);
        node *get_parent_node();
        void render() const;

    private:
        void init();
        node parent_node;
};

#endif // SCENE_H
