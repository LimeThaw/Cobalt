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
        void append_node(node_ptr new_child);
        void append_mesh(const std::string &file_path);
        node_ptr get_parent_node();
        std::vector<node_ptr> enumerate_nodes();
        node_ptr find_node(string name);

    private:
        void init();
        node_ptr parent_node;
};

#endif // SCENE_H
