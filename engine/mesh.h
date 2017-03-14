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
#include "pointer_wrapper.h"

typedef pointer_wrapper<mesh> mesh_ptr;

#define MESH_INFO false     //Toggle detailed information output
//when loading meshes

struct vertex_data {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 normal;
};

class mesh : public node {
    public:
        mesh();
        mesh(const std::string &file_path);
        mesh(const std::string &file_path, const std::string &arg_name);
        ~mesh();
        bool load_model(const std::string &model_path);
        bool load_model(const std::string &scene_path, int model_index);
        void set_material(std::shared_ptr<material> material);
        std::shared_ptr<material> get_material() const;
        void render();
        void render_no_bind();
        bool is_shadow_caster();
        bounding_box get_bounding_box();
        const string &get_path();

    private:
        void load_model(aiMesh *inmesh);
		void buffer_vertices(glm::mat4 *bone_transformations = nullptr, bool keep_size = false);
	    std::vector<vertex_data> vertices;
	    std::vector<glm::vec3> faces;
        GLfloat *vertex_buffer;
        GLfloat *uv_buffer;
        GLfloat *normal_buffer;
        GLfloat *tangent_buffer;
        unsigned int vertex_count;
        unsigned int uv_count;
        unsigned int normal_count;
        unsigned int tangent_count;
        GLuint vertex_array_object_id;
        GLuint vertex_id;
        GLuint uv_id;
        GLuint normal_id;
        GLuint tangent_id;
        material_ptr mat;
        bool has_uvs;
        bounding_box box;
        string path;
};

#endif // MESH_H
