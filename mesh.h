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

#include "material_manager.h"
#include "camera_manager.h"

#define MESH_INFO false     //Toggle detailed information output
                            //when loading meshes


class mesh{
    public:
        mesh();
        ~mesh();
        bool load_model(const char* model_path);
        bool load_model(const char* scene_path, int model_index);
        void set_material(unsigned int new_materialID);
        void place(float x, float y, float z);
        void set_orientation(float x, float y, float z);
        void set_scale(float new_scale);
        void render(glm::mat4 parent_matrix, glm::mat4 parent_rotation_matrix);

    private:
        bool load_model(aiMesh* inmesh);
        GLfloat* vertex_data;
        GLfloat* uv_data;
        GLfloat* normal_data;
        unsigned int vertex_count;
        unsigned int uv_count;
        unsigned int normal_count;
        GLuint vertexID;
        GLuint uvID;
        GLuint normalID;
        glm::mat4 location;
        glm::mat4 rotation;
        glm::mat4 scale;
        glm::mat4 model;
        int materialID;
        bool has_uvs;
};

#endif // MESH_H
