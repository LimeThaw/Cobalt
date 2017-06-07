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
#include "named.h"
#include "pointer_wrapper.h"

typedef pointer_wrapper<node> node_ptr;

class mesh;

/**
 *  General node in the scene graph. In this form it is an abstract node used to groug objects
 *  together and transform them as a group. All elements of the scene that have a position
 *  should implement this class to inherit its functionality and enable its introduction into
 *  the scene graph. Meshes, point lights ant cameras are also nodes for those reasons.
 */
class node : public named, public std::enable_shared_from_this<node> {
    public:
        node(std::string name = "");
        node(const std::string &scene_path, std::string name);
        virtual ~node();
        void load_model(const std::string &path);
        bool load_scene(const std::string &path);
        virtual void set_material(std::shared_ptr<material> new_material);
        void place(float x, float y, float z);
        void place(glm::vec3 arg_position);
        void move(float x, float y, float z);
        void move(glm::vec3 arg_movement);
        void move_relative(float x, float y, float z);
        void rotate(float x, float y, float z);
        void set_scale(float x, float y, float z);
        void set_scale(float new_scale);
        void look_at(float x, float y, float z, glm::vec3 up_vector = glm::vec3(0, 1, 0));
        void look_at(glm::vec3 arg_look, glm::vec3 up_vector = glm::vec3(0, 1, 0));
        void append_node(const std::string &file_path);
        void append_node(node_ptr new_child);
        void append_mesh(const std::string &file_path);
        void set_parent(node_ptr new_parent);
        node_ptr get_parent();
        bool remove_child(node_ptr child);
        glm::mat4 get_node_matrix() const;
        glm::mat4 get_isolated_matrix() const;
        void set_node_matrix(glm::mat4 matrix);
        std::vector<node_ptr> enumerate();
        std::vector<node_ptr> get_children();

    protected:
        weak_ptr<node> parent_node;
        void load_model(const std::string &path, int model_index);
        glm::mat4 node_matrix;
        std::vector<node_ptr> children;
};

#endif // NODE_H
