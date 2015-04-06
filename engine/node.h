//Class containing a group of mesh objects that can be transformed and rendered as a group
//Can have other node objects as children or parents

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include "mesh.h"

class node {
    public:
        node();
        node(const std::string &scene_path);
        ~node();
        void load_model(const std::string &path);
        bool load_scene(const std::string &path);
        void set_material(unsigned int new_material_id);
        void place(float x, float y, float z);
        void set_orientation(float x, float y, float z);
        void set_scale(float x, float y, float z);
        void set_scale(float new_scale);
        void append_node(const std::string &file_path);
        void append_node(node *new_child);
        void set_parent(node *new_parent);
        bool remove_child(node *child);
        glm::mat4 get_node_matrix() const;
        void render() const;
        void render(glm::mat4 parent_matrix);
        std::vector< node*> enumerate();
        const std::vector< mesh *> &get_models();

    private:
        node *parent_node;
        void load_model(const std::string &path, int model_index);
        glm::mat4 location;
        glm::mat4 rotation;
        glm::mat4 scale;
        glm::mat4 node_matrix;
        std::vector<mesh *> models;
        std::vector<node *> children;
};

#endif // NODE_H
