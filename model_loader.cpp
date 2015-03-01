
#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <iostream>

struct mesh {
    std::vector<glm::vec3> vertices;
};

bool load_model(const std::string &file_name) {
    Assimp::Importer importer;

    const aiScene *scene = importer.ReadFile(file_name,
                           aiProcess_CalcTangentSpace |
                           aiProcess_Triangulate |
                           aiProcess_JoinIdenticalVertices |
                           aiProcess_SortByPType |
                           aiProcess_GenSmoothNormals);

    if(!scene) {
        std::cerr << importer.GetErrorString();
        return false;
    }

    for(int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh *inmesh = scene->mMeshes[i];
        mesh retmesh;
        for(int j = 0; j < (inmesh->mNumVertices); j++) {
            glm::vec3 vertex = glm::vec3(inmesh->mVertices[j].x, inmesh->mVertices[j].y, inmesh->mVertices[j].z);
            retmesh.vertices.push_back(vertex);
        }
    }

    //DoTheSceneProcessing( scene);

    return true;
}
