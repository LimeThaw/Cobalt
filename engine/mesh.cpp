#include "mesh.h"
#include "gl_exception.h"
#include "shader.h"

mesh::mesh() : node() {
    node_matrix = glm::mat4(1.0f);
    parent_node = nullptr;
    vertex_data = nullptr;
    uv_data = nullptr;
    tangent_data = nullptr;
    normal_data = nullptr;
    vertex_id = uv_id = tangent_id = normal_id = 0;
    vertex_array_object_id = 0;
    has_uvs = false;
    box = bounding_box();
    path = "";

}

mesh::mesh(const std::string &file_path) : mesh() {
	load_model(file_path);
}

mesh::mesh(const std::string &file_path, const std::string &arg_name) : mesh(file_path) {
	set_name(arg_name);
}

mesh::~mesh() {
    if(vertex_data != nullptr) delete[] vertex_data;
    if(uv_data != nullptr) delete[] uv_data;
    if(tangent_data != nullptr) delete[] tangent_data;
    if(normal_data != nullptr) delete[] normal_data;
    if(vertex_array_object_id != 0) glDeleteVertexArrays(1, &vertex_array_object_id);
    if(vertex_id != 0) glDeleteBuffers(1, &vertex_id);
    if(uv_id != 0) glDeleteBuffers(1, &uv_id);
    if(tangent_id != 0)glDeleteBuffers(1, &tangent_id);
    if(normal_id != 0) glDeleteBuffers(1, &normal_id);
}

bool mesh::load_model(const std::string &model_path) {
    return load_model(model_path, 0);
}

bool mesh::load_model(const std::string &scene_path, int model_index) {
    std::clog << indent::get() << "- Loading mesh " << scene_path << " [" << model_index << "]\n";
	indent::increase();
    float start_time = glfwGetTime();

    Assimp::Importer importer;      //Create importer object
    const aiScene *scene = importer.ReadFile(scene_path,
                           aiProcess_CalcTangentSpace |
                           aiProcess_Triangulate |
                           aiProcess_JoinIdenticalVertices |
                           aiProcess_SortByPType |
                           aiProcess_GenSmoothNormals);    //Load scene

    if(!scene) {        //Check for loading errors
        std::cerr << indent::get() << "! Failed to load model " << scene_path << ": " << importer.GetErrorString();
        return false;
    }

    load_model(scene->mMeshes[model_index]);

    glGenBuffers(1, &vertex_id);      //Buffer vertices
    glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(GLfloat), vertex_data, GL_STATIC_DRAW);

    if(has_uvs) {
        glGenBuffers(1, &uv_id);      //Buffer uvs
        glBindBuffer(GL_ARRAY_BUFFER, uv_id);
        glBufferData(GL_ARRAY_BUFFER, uv_count * sizeof(GLfloat), uv_data, GL_STATIC_DRAW);

        glGenBuffers(1, &tangent_id);       //Buffer tangents
        glBindBuffer(GL_ARRAY_BUFFER, tangent_id);
        glBufferData(GL_ARRAY_BUFFER, tangent_count * sizeof(GLfloat), tangent_data, GL_STATIC_DRAW);
    }

    glGenBuffers(1, &normal_id);      //Buffer normals
    glBindBuffer(GL_ARRAY_BUFFER, normal_id);
    glBufferData(GL_ARRAY_BUFFER, normal_count * sizeof(GLfloat), normal_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vertex_array_object_id);
    glBindVertexArray(vertex_array_object_id);

    glEnableVertexAttribArray(shader_vertex_location);       //Give vertices to OGL (location = 0)
    glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
    glVertexAttribPointer(
        shader_vertex_location,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void *)0
    );

    if(has_uvs) {
        glEnableVertexAttribArray(shader_uv_location);       //Give uvs to OGL (location = 1)
        glBindBuffer(GL_ARRAY_BUFFER, uv_id);
        glVertexAttribPointer(
            shader_uv_location,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void *)0
        );

        glEnableVertexAttribArray(shader_tangent_location);       //Give tangents to OGL (location = 3)
        glBindBuffer(GL_ARRAY_BUFFER, tangent_id);
        glVertexAttribPointer(
            shader_tangent_location,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void *)0
        );
    }

    glEnableVertexAttribArray(shader_normal_location);       //Give normals to OGL (location = 2)
    glBindBuffer(GL_ARRAY_BUFFER, normal_id);
    glVertexAttribPointer(
        shader_normal_location,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void *)0
    );

    glBindVertexArray(0);

	path = scene_path;

    if(DEBUG_INFO) std::clog << indent::get() << "- Finished loading mesh " /*<< model_path*/ << " with " /*<< vertices.size() << " vertices and " */ << vertex_count / 3 << " triangles in " << glfwGetTime() - start_time << " seconds\n\n";
	indent::decrease();

    return true;        //Function finished properly
}

void mesh::set_material(std::shared_ptr<material> material) {
    mat = material;
}

std::shared_ptr<material> mesh::get_material() const {
    return mat;
}

void mesh::render() {
    if(mat) {
        mat->use();
    } else {
        std::cerr << indent::get() << "! Tried to render model without assigned material\n";
        return;
    }

    glBindVertexArray(vertex_array_object_id);

	// Debug error Checking
	if(DEBUG_INFO) {
		GLint shader_id;
		glGetIntegerv(GL_CURRENT_PROGRAM, &shader_id);
		glValidateProgram(shader_id);
		GLsizei length;
		char log_buffer[512];
		glGetProgramInfoLog(shader_id, 512, &length, log_buffer);
		if(length > 0) std::cerr << indent::get() << "! Shader error: " << log_buffer << '\n';
	}

    glDrawArrays(GL_TRIANGLES, 0, vertex_count / 3);    //draw the mesh

    glBindVertexArray(0);

	mat->unbind();
}

void mesh::render_no_bind() {
    glBindVertexArray(vertex_array_object_id);

	// Debug error Checking
	if(DEBUG_INFO) {
		GLint shader_id;
	    glGetIntegerv(GL_CURRENT_PROGRAM, &shader_id);
		glValidateProgram(shader_id);
		GLsizei length;
		char log_buffer[512];
		glGetProgramInfoLog(shader_id, 512, &length, log_buffer);
		if(length > 0) std::cerr << indent::get() << "! Shader error: " << log_buffer << '\n';
	}

    glDrawArrays(GL_TRIANGLES, 0, vertex_count / 3);    //draw the mesh

    glBindVertexArray(0);
}

bool mesh::is_shadow_caster() {
	return ((bool)mat && mat->is_shadow_caster());
}

bounding_box mesh::get_bounding_box() {
	return box;
}

const string &mesh::get_path() {
	return path;
}

//Private
void mesh::load_model(aiMesh *inmesh) {

    if(MESH_INFO)std::clog << indent::get() << "- Loading model\n";

    glm::vec3 min_vertex = glm::vec3(0, 0, 0), max_vertex = glm::vec3(0, 0, 0);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> faces;

    for(unsigned int i = 0; i < inmesh->mNumVertices; i++) { //Get vertices
        glm::vec3 vertex = glm::vec3(inmesh->mVertices[i].x, inmesh->mVertices[i].y, inmesh->mVertices[i].z);     //Get vertices and push them into vector
        vertices.push_back(vertex);

        if(vertex.x < min_vertex.x) min_vertex.x = vertex.x;
        if(vertex.x > max_vertex.x) max_vertex.x = vertex.x;
        if(vertex.y < min_vertex.y) min_vertex.y = vertex.y;
        if(vertex.y > max_vertex.y) max_vertex.y = vertex.y;
        if(vertex.z < min_vertex.z) min_vertex.z = vertex.z;
        if(vertex.z > max_vertex.z) max_vertex.z = vertex.z;

        if(MESH_INFO)std::clog << indent::get() << "- Read Vertex [" << i << "] at " << vertex.x << ", " << vertex.y << ", " << vertex.z << '\n';
    }
    box = bounding_box(min_vertex, max_vertex);

    if(inmesh->HasTextureCoords(0)) {    //Get uvs
        has_uvs = true;
        for(unsigned int i = 0; i < inmesh->mNumVertices; i++) {
            glm::vec2 uv = glm::vec2(inmesh->mTextureCoords[0][i].x, inmesh->mTextureCoords[0][i].y);
            uvs.push_back(uv);
            if(MESH_INFO)std::clog << indent::get() << "- Read UV [" << i << "] at " << uv.x << ", " << uv.y << '\n';
        }
    } else {
        has_uvs = false;
        if(DEBUG_INFO) std::cerr << indent::get() << "! Could not find texture coordinates or generate tangents\n";
    }

    if(inmesh->HasNormals()) {      //Get normals
        for(unsigned int i = 0; i < inmesh->mNumVertices; i++) {
            glm::vec3 normal = glm::vec3(inmesh->mNormals[i].x, inmesh->mNormals[i].y, inmesh->mNormals[i].z);
            normals.push_back(normal);
            if(MESH_INFO)std::clog << indent::get() << "- Read Normal [" << i << "] at " << normal.x << ", " <<  normal.y << ", " << normal.z << '\n';
        }
    } else {
        if(DEBUG_INFO) std::cerr << indent::get() << "! Could not find normals\n";
    }

    if(inmesh->HasTangentsAndBitangents()) {      //Get tangents
        for(unsigned int i = 0; i < inmesh->mNumVertices; i++) {
            glm::vec3 tangent = glm::vec3(inmesh->mTangents[i].x, inmesh->mTangents[i].y, inmesh->mTangents[i].z);
            tangents.push_back(tangent);
            if(MESH_INFO)std::clog << indent::get() << "- Read Tangent [" << i << "] at " << tangent.x << ", " <<  tangent.y << ", " << tangent.z << '\n';
        }
    } else {
        //std::cerr << "   ! Could not generate tangents\n";
    }

    for(unsigned int i = 0; i < inmesh->mNumFaces; i++) { //Get faces
        aiFace face = inmesh->mFaces[i];
        glm::vec3 vec = glm::vec3(face.mIndices[0], face.mIndices[1], face.mIndices[2]);    //Get faces and put them into vector
        faces.push_back(vec);
        if(MESH_INFO)std::clog << indent::get() << "- Read face [" << i << "] with vertices " << vec.x << ", " << vec.y << ", " << vec.z << '\n';
    }

    //Put data into float arrays
    std::vector<GLfloat> vdata;     //vertices
    std::vector<GLfloat> udata;     //uvs
    std::vector<GLfloat> tdata;     //tangents
    std::vector<GLfloat> ndata;     //normals
    for(unsigned int i = 0; i < faces.size(); i++) {
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

        if(has_uvs) {
            udata.push_back(uvs[face.x].x);
            udata.push_back(uvs[face.x].y);
            udata.push_back(uvs[face.y].x);
            udata.push_back(uvs[face.y].y);
            udata.push_back(uvs[face.z].x);
            udata.push_back(uvs[face.z].y);

            tdata.push_back(tangents[face.x].x);
            tdata.push_back(tangents[face.x].y);
            tdata.push_back(tangents[face.x].z);
            tdata.push_back(tangents[face.y].x);
            tdata.push_back(tangents[face.y].y);
            tdata.push_back(tangents[face.y].z);
            tdata.push_back(tangents[face.z].x);
            tdata.push_back(tangents[face.z].y);
            tdata.push_back(tangents[face.z].z);
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

    if(vertex_data != nullptr)delete[] vertex_data;    //Put vertices in object memory
    vertex_data = new GLfloat[vdata.size()];
    std::copy(vdata.begin(), vdata.end(), vertex_data);
    vertex_count = vdata.size();
    vdata.clear();

    if(has_uvs) {
        if(uv_data != nullptr)delete[] uv_data;    //Put uvs in object memory if present
        uv_data = new GLfloat[udata.size()];
        std::copy(udata.begin(), udata.end(), uv_data);
        uv_count = udata.size();

        if(tangent_data != nullptr)delete[] tangent_data;    //Put tangents in object memory
        tangent_data = new GLfloat[tdata.size()];
        std::copy(tdata.begin(), tdata.end(), tangent_data);
        tangent_count = tdata.size();
    } else {
        uv_data = nullptr;
        tangent_data = nullptr;
    }
    udata.clear();
    tdata.clear();

    if(normal_data != nullptr)delete[] normal_data;    //Put normals in object memory
    normal_data = new GLfloat[ndata.size()];
    std::copy(ndata.begin(), ndata.end(), normal_data);
    normal_count = ndata.size();
    ndata.clear();

    vertices.clear();   //Tidy up
    uvs.clear();
    tangents.clear();
    normals.clear();
    faces.clear();
}
