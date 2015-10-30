/**
Class containing a group of mesh objects that can be transformed and rendered as a group.
Can have other node objects as children or parents.
*/

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <vector>
#include "mesh.h"

class node {
    public:
        node();///< Default constructor.
        node(const std::string &scene_path);///< Constructor loading all meshes from a file and appending them to the node.
        ~node();///< Default destructor.
        void load_model(const std::string &path);///< Loads the first mesh in the specified file and appends it to the node.
        bool load_scene(const std::string &path);///< Loads all meshes from the specified file and appends them to the node.
        void set_material(std::shared_ptr<material> new_material);///< Sets the material for all nodes and meshes appended to this node.
        void place(float x, float y, float z);///< Specifies the node location relative to its parent node.
        void place(glm::vec3 arg_position);///< See member place(float x, float y, float z).
        void move(float x, float y, float z);///< Moves the node relative to its parent node.
        void move(glm::vec3 arg_movement);///< See member move(float x, float y, float z).
        void move_relative(float x, float y, float z);///< Moves the node relative to its parent considering the node's rotation.
        void set_orientation(float x, float y, float z);///< Sets the node's rotation relative to its parent.
        void set_scale(float x, float y, float z);///< Sets the node's scale.
        void set_scale(float new_scale);///< See member set_scale(float x, float y, float z).
        void append_node(const std::string &file_path);///< Basically append_node(new node(file_path)).
        void append_node(node *new_child);///< Appends the given node as its child.
        void set_parent(node *new_parent);///< Set the node's parent.
        bool remove_child(node *child);///< Removes a child node from this node.
        glm::mat4 get_node_matrix() const;///< Returns the transformation matrix affecting all of the node's children. It includes the transformation of it's parents.
        void render(glm::mat4 view_matrix) const;///< Renders the whole node with all of its descendants.
        void render(glm::mat4 parent_matrix, glm::mat4 view_matrix);///< Renders the whole node with special transformation and view matrices.
        std::vector< node*> enumerate();///< Returns a list of itself and all its child nodes.
        const std::vector< mesh *> &get_models();///< Returns a list of all mesh objects attached to this node.

    private:
        node *parent_node;
        void load_model(const std::string &path, int model_index);
        glm::mat4 node_matrix;
        std::vector<mesh *> models;
        std::vector<node *> children;
};

#endif // NODE_H
