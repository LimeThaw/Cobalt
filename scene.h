#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "node.h"

class scene{
    public:
        scene();
        ~scene();
        void append_node(const char* file_path);
        void append_node(node* new_child);
        node* get_parent_node();
        void render();

    private:
        void init();
        node parent_node;
};

#endif // SCENE_H
