#include "mesh.h"

mesh::mesh(){
    model = location = rotation = scale = glm::mat4(1.0f);
    vertex_data = NULL;
    uv_data = NULL;
    normal_data = NULL;
    vertexID = uvID = normalID = NULL;
    materialID = -1;
    has_uvs = false;
}

mesh::~mesh(){
    if(vertex_data!=NULL)delete[] vertex_data;
    if(uv_data!=NULL)delete[] uv_data;
    if(normal_data!=NULL)delete[] normal_data;
    if(vertexID!=NULL)glDeleteBuffers(1, &vertexID);
    if(uvID!=NULL)glDeleteBuffers(1, &uvID);
    if(normalID!=NULL)glDeleteBuffers(1, &normalID);
    if(materialID!=-1)remove_material_instance(materialID);
}

bool mesh::load_model(const char* model_path){
    load_model(model_path, 0);
}

bool mesh::load_model(const char* scene_path, int model_index){
    std::clog << "-Loading mesh " << scene_path << " [" << model_index << "]\n";
    float start_time = glfwGetTime();

    Assimp::Importer importer;      //Create importer object
    const aiScene* scene = importer.ReadFile(scene_path,
    aiProcess_CalcTangentSpace |
    aiProcess_Triangulate |
    aiProcess_JoinIdenticalVertices |
    aiProcess_SortByPType |
    aiProcess_GenSmoothNormals);    //Load scene

    if( !scene){        //Check for loading errors
        std::cerr << "-Failed to load model " << scene_path << ": " << importer.GetErrorString();
        return false;
    }

    load_model(scene->mMeshes[model_index]);

    glGenBuffers(1, &vertexID);      //Buffer vertices
    glBindBuffer(GL_ARRAY_BUFFER, vertexID);
    glBufferData(GL_ARRAY_BUFFER, vertex_count*sizeof(GLfloat), vertex_data, GL_STATIC_DRAW);

    if(has_uvs){
        glGenBuffers(1, &uvID);      //Buffer uvs
        glBindBuffer(GL_ARRAY_BUFFER, uvID);
        glBufferData(GL_ARRAY_BUFFER, uv_count*sizeof(GLfloat), uv_data, GL_STATIC_DRAW);
    }

    glGenBuffers(1, &normalID);      //Buffer normals
    glBindBuffer(GL_ARRAY_BUFFER, normalID);
    glBufferData(GL_ARRAY_BUFFER, normal_count*sizeof(GLfloat), normal_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::clog << "- Finished loading mesh " /*<< model_path*/ << " with " /*<< vertices.size() << " vertices and " */<< vertex_count/3 << " triangles in " << glfwGetTime()-start_time << "seconds\n\n";

    return true;        //Function finished properly
}

void mesh::set_material(unsigned int new_materialID){
    materialID = new_materialID;
}

void mesh::place(float x, float y, float z){
    location = glm::translate(glm::vec3(x, y, z));
}

void mesh::set_orientation(float x, float y, float z){
    glm::quat rot = glm::quat(glm::vec3(x*0.017453294, y*0.017453294, z*0.017453294));
    rotation = glm::mat4_cast(rot);
}

void mesh::set_scale(float new_scale){
    scale = glm::scale(glm::vec3(new_scale));
}

void mesh::render(glm::mat4 parent_matrix, glm::mat4 parent_rotation_matrix){

    if(materialID != -1){
        set_active_material(materialID);
    }else{
        std::cerr << "! Tried to render model without assigned material\n";
        return;
    }

    update_camera();

    model = location * rotation * scale;        //Calculate and register the model and rotation matrices
    glm::mat4 render_rotation = parent_rotation_matrix * rotation;
    glm::mat4 render_model = parent_matrix * location * rotation * scale;
    GLint shaderID;
    glGetIntegerv(GL_CURRENT_PROGRAM, &shaderID);
    GLuint matrixID = glGetUniformLocation(shaderID, "model");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &render_model[0][0]);
    matrixID = glGetUniformLocation(shaderID, "rotation");
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &render_rotation[0][0]);

    glEnableVertexAttribArray(0);       //Give vertices to OGL (location = 0)
    glBindBuffer(GL_ARRAY_BUFFER, vertexID);
    glVertexAttribPointer(
       0,
       3,
       GL_FLOAT,
       GL_FALSE,
       0,
       (void*)0
    );

    if(has_uvs){
        glEnableVertexAttribArray(1);       //Give uvs to OGL (location = 1)
        glBindBuffer(GL_ARRAY_BUFFER, uvID);
        glVertexAttribPointer(
           1,
           2,
           GL_FLOAT,
           GL_FALSE,
           0,
           (void*)0
        );
    }

    glEnableVertexAttribArray(2);       //Give normals to OGL (location = 2)
    glBindBuffer(GL_ARRAY_BUFFER, normalID);
    glVertexAttribPointer(
       2,
       3,
       GL_FLOAT,
       GL_FALSE,
       0,
       (void*)0
    );

    glDrawArrays(GL_TRIANGLES, 0, vertex_count/3);      //draw the mesh

    glDisableVertexAttribArray(2);
    if(has_uvs) glDisableVertexAttribArray(1);      //Clean up
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

//Private
bool mesh::load_model(aiMesh* inmesh){

    std::clog << "- Loading model\n";

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> faces;

    for(int i=0; i<inmesh->mNumVertices; i++){      //Get vertices
        glm::vec3 vertex = glm::vec3(inmesh->mVertices[i].x, inmesh->mVertices[i].y, inmesh->mVertices[i].z);     //Get vertices and push them into vector
        vertices.push_back(vertex);
        if(MESH_INFO)std::clog << "- Read Vertex [" << i <<"] at " << vertex.x << ", " << vertex.y << ", " << vertex.z << '\n';
    }

    if(inmesh->HasTextureCoords(0)){     //Get uvs
        has_uvs = true;
        for(int i=0; i<inmesh->mNumVertices; i++){
            glm::vec2 uv = glm::vec2(inmesh->mTextureCoords[0][i].x, inmesh->mTextureCoords[0][i].y);
            uvs.push_back(uv);
            if(MESH_INFO)std::clog << "- Read UV [" << i << "] at " << uv.x << ", " << uv.y << '\n';
        }
    }else{
        has_uvs = false;
        std::cerr << "! Could not find texture coordinates\n";
    }

    if(inmesh->HasNormals()){       //Get normals
        for(int i=0; i<inmesh->mNumVertices; i++){
            glm::vec3 normal = glm::vec3(inmesh->mNormals[i].x, inmesh->mNormals[i].y, inmesh->mNormals[i].z);
            normals.push_back(normal);
            if(MESH_INFO)std::clog << "- Read Normal [" << i << "] at " << normal.x << ", " <<  normal.y << ", " << normal.z << '\n';
        }
    }else{
        std::cerr << "- Could not find normals\n";
    }

    for(int i=0; i<inmesh->mNumFaces; i++){     //Get faces
        aiFace face = inmesh->mFaces[i];
        glm::vec3 vec = glm::vec3(face.mIndices[0], face.mIndices[1], face.mIndices[2]);    //Get faces and put them into vector
        faces.push_back(vec);
        if(MESH_INFO)std::clog << "- Read face [" << i << "] with vertices " << vec.x << ", " << vec.y << ", " << vec.z << '\n';
    }

    //Put data into float arrays
    std::vector<GLfloat> vdata;     //vertices
    std::vector<GLfloat> udata;     //uvs
    std::vector<GLfloat> ndata;     //normals
    for(int i=0; i<faces.size(); i++){
        glm::vec3 face = faces[i];
        vdata.push_back(vertices[face.x].x);
        vdata.push_back(vertices[face.x].y);
        vdata.push_back(vertices[face.x].z);
        vdata.push_back(vertices[face.y].x);
        vdata.push_back(vertices[face.y].y);
        vdata.push_back(vertices[face.y].z);
        vdata.push_back(vertices[face.z].x);
        vdata.push_back(vertices[face.z].y);
        vdata.push_back(vertices[face.z].z);
        if(has_uvs){
            udata.push_back(uvs[face.x].x);
            udata.push_back(uvs[face.x].y);
            udata.push_back(uvs[face.y].x);
            udata.push_back(uvs[face.y].y);
            udata.push_back(uvs[face.z].x);
            udata.push_back(uvs[face.z].y);
        }
        ndata.push_back(normals[face.x].x);
        ndata.push_back(normals[face.x].y);
        ndata.push_back(normals[face.x].z);
        ndata.push_back(normals[face.y].x);
        ndata.push_back(normals[face.y].y);
        ndata.push_back(normals[face.y].z);
        ndata.push_back(normals[face.z].x);
        ndata.push_back(normals[face.z].y);
        ndata.push_back(normals[face.z].z);
    }

    if(vertex_data!=NULL)delete[] vertex_data;      //Put vertices in object memory
    vertex_data = new GLfloat[vdata.size()];
    std::copy(vdata.begin(), vdata.end(), vertex_data);
    vertex_count = vdata.size();
    vdata.clear();

    if(uv_data!=NULL)delete[] uv_data;      //Put uvs in object memory if present
    if(has_uvs){
        uv_data = new GLfloat[udata.size()];
        std::copy(udata.begin(), udata.end(), uv_data);
        uv_count = udata.size();
    }else{
        uv_data = NULL;
    }
    udata.clear();

    if(normal_data!=NULL)delete[] normal_data;      //Put normals in object memory
    normal_data = new GLfloat[ndata.size()];
    std::copy(ndata.begin(), ndata.end(), normal_data);
    normal_count = ndata.size();
    ndata.clear();

    vertices.clear();   //Tidy up
    uvs.clear();
    normals.clear();
    faces.clear();
}
