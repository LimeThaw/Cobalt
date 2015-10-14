//Class defining an object to be rendered
//Should be rendered as a part of the scene graph

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "material.h"

#define MESH_INFO false     //Toggle detailed information output
//when loading meshes


class mesh {
    public:
        mesh();
        ~mesh();
        bool load_model(const std::string &model_path);
        bool load_model(const std::string &scene_path, int model_index);
        void set_material(std::shared_ptr<material> mat);
        void place(float x, float y, float z);
        void set_orientation(float x, float y, float z);
        void set_scale(float new_scale);
        void render(glm::mat4 parent_matrix, glm::mat4 view_matrix);
        std::shared_ptr<material> get_material() const;

    private:
        void load_model(aiMesh *inmesh);
        GLfloat *vertex_data;
        GLfloat *uv_data;
        GLfloat *normal_data;
        GLfloat *tangent_data;
        unsigned int vertex_count;
        unsigned int uv_count;
        unsigned int normal_count;
        unsigned int tangent_count;
        GLuint vertex_array_object_id;
        GLuint vertex_id;
        GLuint uv_id;
        GLuint normal_id;
        GLuint tangent_id;
        glm::mat4 location;
        glm::mat4 rotation;
        glm::mat4 scale;
        glm::mat4 model;
        std::shared_ptr<material> mat;
        bool has_uvs;
};

#endif // MESH_H
