//Class defining an object to be rendered
//Should be rendered as a part of the scene graph

#ifndef MESH_H
#define MESH_H

#define  GLM_FORCE_RADIANS

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
#include "node.h"
#include "scene.h"
#include "bounding_box.h"

#define MESH_INFO false     //Toggle detailed information output
//when loading meshes


class mesh : public node {
    public:
        mesh();
        mesh(const std::string &file_path);
        ~mesh();
        bool load_model(const std::string &model_path);
        bool load_model(const std::string &scene_path, int model_index);
        void set_material(std::shared_ptr<material> material);
        std::shared_ptr<material> get_material() const;
        void render();
        void render_no_bind();
        bool is_shadow_caster();
        bounding_box get_bounding_box();

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
        std::shared_ptr<material> mat;
        bool has_uvs;
        bounding_box box;
};

#endif // MESH_H
