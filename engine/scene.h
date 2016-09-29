//Class describing the whole scene to be rendered
//parent_node should be the topmost node of the scene graph

#ifndef SCENE_H
#define SCENE_H

#define  GLM_FORCE_RADIANS

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "node.h"
#include "util.h"

class scene {
    public:
        scene();
        ~scene();
        void append_node(const std::string &file_path);
        void append_node(node *new_child);
        void append_mesh(const std::string &file_path);
        node *get_parent_node();
        std::vector< node * > enumerate_nodes();

    private:
        void init();
        node parent_node;
};

#endif // SCENE_H
