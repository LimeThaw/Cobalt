/**
Class containing a group of mesh objects that can be transformed and rendered as a group.
Can have other node objects as children or parents.
*/

#ifndef NODE_H
#define NODE_H

#define  GLM_FORCE_RADIANS

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <vector>
#include <memory>

#include "material.h"

class mesh;

class node {
    public:
        node();///< Default constructor.
        node(const std::string &scene_path);///< Constructor loading all meshes from a file and appending them to the node.
        virtual ~node();///< Default destructor.
        void load_model(const std::string &path);///< Loads the first mesh in the specified file and appends it to the node.
        bool load_scene(const std::string &path);///< Loads all meshes from the specified file and appends them to the node.
        virtual void set_material(std::shared_ptr<material> new_material);///< Sets the material for all nodes and meshes appended to this node.
        void place(float x, float y, float z);///< Specifies the node location relative to its parent node.
        void place(glm::vec3 arg_position);///< See member place(float x, float y, float z).
        void move(float x, float y, float z);///< Moves the node relative to its parent node.
        void move(glm::vec3 arg_movement);///< See member move(float x, float y, float z).
        void move_relative(float x, float y, float z);///< Moves the node relative to its parent considering the node's rotation.
        void rotate(float x, float y, float z);///< Rotates the node for the given angles around the respective axes.
        void set_scale(float x, float y, float z);///< Sets the node's scale.
        void set_scale(float new_scale);///< See member set_scale(float x, float y, float z).
        void look_at(float x, float y, float z, glm::vec3 up_vector = glm::vec3(0, 1, 0));///< Rotates the node to face the specified point in space.
        void look_at(glm::vec3 arg_look, glm::vec3 up_vector = glm::vec3(0, 1, 0));///< See member look_at(float x, float y, float z).
        void append_node(const std::string &file_path);///< Basically append_node(new node(file_path)).
        void append_node(node *new_child);///< Appends the given node as its child.
        void append_mesh(const std::string &file_path);
        void set_parent(node *new_parent);///< Set the node's parent.
        node* get_parent();
        bool remove_child(node *child);///< Removes a child node from this node.
        glm::mat4 get_node_matrix() const;///< Returns the transformation matrix affecting all of the node's children. It includes the transformation of it's parents.
        glm::mat4 get_isolated_matrix() const;///< Returns the matrix describing the transformation of this node relative to its parent.
        void set_node_matrix(glm::mat4 matrix);///< Sets the matrix describing the node's relative transformation.
        std::vector< node*> enumerate();///< Returns a list of itself and all its child nodes.
        std::vector<node*> get_children();
        string set_name(const string arg_name);
        string get_name();

    protected:
        node *parent_node;
        void load_model(const std::string &path, int model_index);
        glm::mat4 node_matrix;
        std::vector<node *> children;
        string name;
};

#endif // NODE_H
