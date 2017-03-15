#include "mesh.h"
#include "gl_exception.h"
#include "shader.h"

mesh::mesh() : node() {
    node_matrix = glm::mat4(1.0f);
    parent_node = weak_ptr<node>();
    vertex_buffer = nullptr;
    uv_buffer = nullptr;
    tangent_buffer = nullptr;
    normal_buffer = nullptr;
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
    if(vertex_buffer != nullptr) delete[] vertex_buffer;
    if(uv_buffer != nullptr) delete[] uv_buffer;
    if(tangent_buffer != nullptr) delete[] tangent_buffer;
    if(normal_buffer != nullptr) delete[] normal_buffer;
    if(vertex_array_object_id != 0) glDeleteVertexArrays(1, &vertex_array_object_id);
    if(vertex_id != 0) glDeleteBuffers(1, &vertex_id);
    if(uv_id != 0) glDeleteBuffers(1, &uv_id);
    if(tangent_id != 0)glDeleteBuffers(1, &tangent_id);
    if(normal_id != 0) glDeleteBuffers(1, &normal_id);

    vertices.clear();
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

	render_no_bind();

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

	if(inmesh->HasTextureCoords(0)) {
		has_uvs = true;
	} else {
		has_uvs = false;
		if(DEBUG_INFO) std::cerr << indent::get() << "! Could not find texture coordinates or generate tangents\n";
	}

    for(unsigned int i = 0; i < inmesh->mNumVertices; i++) { //Get vertices
		vertex_data vertex;
        vertex.position = glm::vec3(inmesh->mVertices[i].x, inmesh->mVertices[i].y, inmesh->mVertices[i].z); // Read and save vector position...
        if(MESH_INFO)std::clog << indent::get() << "- Read Vertex [" << i << "] at " << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z << '\n';

		if(inmesh->HasTextureCoords(0)) {
	        has_uvs = true;
			vertex.uv = glm::vec2(inmesh->mTextureCoords[0][i].x, inmesh->mTextureCoords[0][i].y); // ... uv coordinates...
			if(MESH_INFO)std::clog << indent::get() << "- Read UV [" << i << "] at " << vertex.uv.x << ", " << vertex.uv.y << '\n';
		}

		if(inmesh->HasNormals()) {
            vertex.normal = glm::vec3(inmesh->mNormals[i].x, inmesh->mNormals[i].y, inmesh->mNormals[i].z); // .. normals...
            if(MESH_INFO)std::clog << indent::get() << "- Read Normal [" << i << "] at " << vertex.normal.x << ", " <<  vertex.normal.y << ", " << vertex.normal.z << '\n';
		} else {
	        if(DEBUG_INFO) std::cerr << indent::get() << "! Could not find normals\n";
		}

		if(inmesh->HasTangentsAndBitangents()) {
			vertex.tangent = glm::vec3(inmesh->mTangents[i].x, inmesh->mTangents[i].y, inmesh->mTangents[i].z);
            if(MESH_INFO)std::clog << indent::get() << "- Read Tangent [" << i << "] at " << vertex.tangent.x << ", " <<  vertex.tangent.y << ", " << vertex.tangent.z << '\n';
		}

		vertices.push_back(vertex);

        if(vertex.position.x < min_vertex.x) min_vertex.x = vertex.position.x;
        if(vertex.position.x > max_vertex.x) max_vertex.x = vertex.position.x;
        if(vertex.position.y < min_vertex.y) min_vertex.y = vertex.position.y;
        if(vertex.position.y > max_vertex.y) max_vertex.y = vertex.position.y;
        if(vertex.position.z < min_vertex.z) min_vertex.z = vertex.position.z;
        if(vertex.position.z > max_vertex.z) max_vertex.z = vertex.position.z;

    }
    box = bounding_box(min_vertex, max_vertex); // Generate AABB for mesh, used for scaling shadow mao frustum

	// Load bones to animate mesh, if present
	for(unsigned int i = 0; i < inmesh->mNumBones; ++i) {
		aiBone *bone = inmesh->mBones[i];
		bones.push_back(aiMatrix4x4ToGlm(&bone->mOffsetMatrix));
		for(unsigned int j = 0; j < bone->mNumWeights; ++j) {
			auto weight = bone->mWeights[j];
			vertices[weight.mVertexId].weights.push_back(weight.mWeight);
		}
	}

    for(unsigned int i = 0; i < inmesh->mNumFaces; i++) { //Get faces
        aiFace face = inmesh->mFaces[i];
        glm::vec3 vec = glm::vec3(face.mIndices[0], face.mIndices[1], face.mIndices[2]);    //Get faces and put them into vector
        faces.push_back(vec);
        if(MESH_INFO)std::clog << indent::get() << "- Read face [" << i << "] with vertices " << vec.x << ", " << vec.y << ", " << vec.z << '\n';
    }

	buffer_vertices();

}

// Puts vertex data in OpenGL-readable float buffers
// Doesn't need to be exectuted if data is already properly buffered
// 		(e.g. in last frame)
void mesh::buffer_vertices(bool keep_size) {
	//Put data into float arrays
	std::vector<GLfloat> vbuffer;     //vertices
	std::vector<GLfloat> ubuffer;     //uvs
	std::vector<GLfloat> tbuffer;     //tangents
	std::vector<GLfloat> nbuffer;     //normals
	for(unsigned int i = 0; i < faces.size(); i++) {
		glm::vec3 face = faces[i];

		vbuffer.push_back(vertices[face.x].position.x);
		vbuffer.push_back(vertices[face.x].position.y);
		vbuffer.push_back(vertices[face.x].position.z);
		vbuffer.push_back(vertices[face.y].position.x);
		vbuffer.push_back(vertices[face.y].position.y);
		vbuffer.push_back(vertices[face.y].position.z);
		vbuffer.push_back(vertices[face.z].position.x);
		vbuffer.push_back(vertices[face.z].position.y);
		vbuffer.push_back(vertices[face.z].position.z);

		if(has_uvs) {
			ubuffer.push_back(vertices[face.x].uv.x);
			ubuffer.push_back(vertices[face.x].uv.y);
			ubuffer.push_back(vertices[face.y].uv.x);
			ubuffer.push_back(vertices[face.y].uv.y);
			ubuffer.push_back(vertices[face.z].uv.x);
			ubuffer.push_back(vertices[face.z].uv.y);

			tbuffer.push_back(vertices[face.x].tangent.x);
			tbuffer.push_back(vertices[face.x].tangent.y);
			tbuffer.push_back(vertices[face.x].tangent.z);
			tbuffer.push_back(vertices[face.y].tangent.x);
			tbuffer.push_back(vertices[face.y].tangent.y);
			tbuffer.push_back(vertices[face.y].tangent.z);
			tbuffer.push_back(vertices[face.z].tangent.x);
			tbuffer.push_back(vertices[face.z].tangent.y);
			tbuffer.push_back(vertices[face.z].tangent.z);
		}

		nbuffer.push_back(vertices[face.x].normal.x);
		nbuffer.push_back(vertices[face.x].normal.y);
		nbuffer.push_back(vertices[face.x].normal.z);
		nbuffer.push_back(vertices[face.y].normal.x);
		nbuffer.push_back(vertices[face.y].normal.y);
		nbuffer.push_back(vertices[face.y].normal.z);
		nbuffer.push_back(vertices[face.z].normal.x);
		nbuffer.push_back(vertices[face.z].normal.y);
		nbuffer.push_back(vertices[face.z].normal.z);
	}

	if(!keep_size) {
		if(vertex_buffer != nullptr)delete[] vertex_buffer;    //Put vertices in object memory
		vertex_buffer = new GLfloat[vbuffer.size()];
	}
	std::copy(vbuffer.begin(), vbuffer.end(), vertex_buffer);
	vertex_count = vbuffer.size();
	vbuffer.clear();

	if(has_uvs) {
		if(!keep_size) {
			if(uv_buffer != nullptr)delete[] uv_buffer;    //Put uvs in object memory if present
			uv_buffer = new GLfloat[ubuffer.size()];
		}
		std::copy(ubuffer.begin(), ubuffer.end(), uv_buffer);
		uv_count = ubuffer.size();

		if(!keep_size) {
			if(tangent_buffer != nullptr)delete[] tangent_buffer;    //Put tangents in object memory
			tangent_buffer = new GLfloat[tbuffer.size()];
		}
		std::copy(tbuffer.begin(), tbuffer.end(), tangent_buffer);
		tangent_count = tbuffer.size();
	} else {
		uv_buffer = nullptr;
		tangent_buffer = nullptr;
	}
	ubuffer.clear();
	tbuffer.clear();

	if(!keep_size) {
		if(normal_buffer != nullptr)delete[] normal_buffer;    //Put normals in object memory
		normal_buffer = new GLfloat[nbuffer.size()];
	}
	std::copy(nbuffer.begin(), nbuffer.end(), normal_buffer);
	normal_count = nbuffer.size();
	nbuffer.clear();

	// Create OpenGL buffers on GPU and buffer data
    if(vertex_id != 0) glDeleteBuffers(1, &vertex_id);
    glGenBuffers(1, &vertex_id);      //Buffer vertices
    glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(GLfloat), vertex_buffer, GL_STATIC_DRAW);

    if(has_uvs) {
    	if(uv_id != 0) glDeleteBuffers(1, &uv_id);
        glGenBuffers(1, &uv_id);      //Buffer uvs
        glBindBuffer(GL_ARRAY_BUFFER, uv_id);
        glBufferData(GL_ARRAY_BUFFER, uv_count * sizeof(GLfloat), uv_buffer, GL_STATIC_DRAW);

		if(tangent_id != 0)glDeleteBuffers(1, &tangent_id);
        glGenBuffers(1, &tangent_id);       //Buffer tangents
        glBindBuffer(GL_ARRAY_BUFFER, tangent_id);
        glBufferData(GL_ARRAY_BUFFER, tangent_count * sizeof(GLfloat), tangent_buffer, GL_STATIC_DRAW);
    }

	if(normal_id != 0) glDeleteBuffers(1, &normal_id);
    glGenBuffers(1, &normal_id);      //Buffer normals
    glBindBuffer(GL_ARRAY_BUFFER, normal_id);
    glBufferData(GL_ARRAY_BUFFER, normal_count * sizeof(GLfloat), normal_buffer, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Generate VAO and give it appropriate values
	if(vertex_array_object_id != 0) glDeleteVertexArrays(1, &vertex_array_object_id);
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
}