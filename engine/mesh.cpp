#include "mesh.h"
#include "gl_exception.h"
#include "shader.h"

/**
 *  Default constructor. Sets all values to zero-equivalent.
 */
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

/**
 *  Loading constructor. Initializes the object and loads the first mesh from the given file.
 *  @param	file_path	The path to the file you want to load the mes from.
 */
mesh::mesh(const std::string &file_path) : mesh() {
	load_model(file_path);
}

/**
 *  Naming constructor. Calls the loading constructor with the first argument and sets the
 *  name of the object to the second argument.
 *  @param	file_path	The path to the file you want to load the mes from.
 *  @param arg_name		The name that you want to assign to the mesh. Should be unique within
 *							the program. See named and name_manager for more detail.
 */
mesh::mesh(const std::string &file_path, const std::string &arg_name) : mesh(file_path) {
	set_name(arg_name);
}

/**
 *  Class destructor.
 */
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

/**
 *  Loads a mesh from the given file. If the file contains more than one model, only the first one
 *  will be loaded. All previous data stored in this object will be deleted.
 *  @param	model_path	The path to the file holding the data.
 */
bool mesh::load_model(const std::string &model_path) {
    return load_model(model_path, 0);
}

/**
 *  Loads the mesh with the specified index from the given file. The file is expected to contain
 *  a sufficient number of models.
 *  @param	scene_path	The path to the file holding the data.
 *  @param	model_index	The index of the model within the given scene.
 */
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

/**
 *  Assigns the given material to this mesh. The material contains all data necessary for
 *  rendering (except for the geometry described in the mesh class). When the mesh is rendered
 *  the assigned material is bound, along with all its textures.
 *  @param	material	A shared_ptr to the material describing the mesh's render properties.
 */
void mesh::set_material(std::shared_ptr<material> material) {
    mat = material;
}

/**
 *  Returns a shared_ptr to the assigned material.
 */
std::shared_ptr<material> mesh::get_material() const {
    return mat;
}

/**
 *  Binds the assigned material and renders the mesh.
 */
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

/**
 *  Renders the mesh without binding a material. Can be handy if the desired material is already
 *  active.
 */
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

/**
 *  Returns true if the material of the mesh casts shadows, false otherwise.
 */
bool mesh::is_shadow_caster() {
	return ((bool)mat && mat->is_shadow_caster());
}

/**
 *  Returns the bounding box of the mesh. The box is usually computed automatically when the mesh
 *  is loaded.
 */
bounding_box mesh::get_bounding_box() {
	return box;
}

/**
 *  Returns the path of the file from which the mesh was loaded. If the mesh was never loaded (e.g.
 *  procedurally generated) it will return the empty string.
 */
const string &mesh::get_path() {
	return path;
}

//Private

/**
 *  Puts the mesh from the given aiMesh into engine format for later rendering. Computes an axis
 *  aligned bounding box and loads rigging and animation data (if implemented yet). Does not read
 *  or save any material, shader or texture data (but does save UV coordinates).
 */
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

/**
 *  Puts vertex data in OpenGL-readable float buffers.
 *  Doesn't need to be exectuted if data is already properly buffered (e.g. in last frame)
 *  @param	keep_size	If this is true, the old data will not be deleted, but simply overwritten
 *							with the new. If the array sizes mismatch there will be weird stuff
 *							going on...
 */
void mesh::buffer_vertices(bool keep_size) {

	// First, we declare local float buffers to get all our data into the right format
	std::vector<GLfloat> vbuffer;     //vertices
	std::vector<GLfloat> ubuffer;     //uvs
	std::vector<GLfloat> tbuffer;     //tangents
	std::vector<GLfloat> nbuffer;     //normals

	// Now we go through all of our faces (which should all be triangles at this point), iterate
	// through their vertices and store their coordinates as three floats in a row. If we give
	// an array like that to OpenGL it will recreate the triangles.
	for(unsigned int i = 0; i < faces.size(); i++) {
		glm::vec3 face = faces[i];

		// face.xyz are the indices of the three vertices of our triangle.
		vbuffer.push_back(vertices[face.x].position.x);
		vbuffer.push_back(vertices[face.x].position.y);
		vbuffer.push_back(vertices[face.x].position.z);
		vbuffer.push_back(vertices[face.y].position.x);
		vbuffer.push_back(vertices[face.y].position.y);
		vbuffer.push_back(vertices[face.y].position.z);
		vbuffer.push_back(vertices[face.z].position.x);
		vbuffer.push_back(vertices[face.z].position.y);
		vbuffer.push_back(vertices[face.z].position.z);

		// If the mesh has UV data, then we also have tangent data (computed if not imported), so
		// we buffer both for later use.
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

		// The normalsare also stored in a separate array.
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

	// If we know that the new data is the same size as the old one, we can simply reuse the memory.
	// Otherwise we delete the old data and reserve new space for our new data.
	if(!keep_size) {
		if(vertex_buffer != nullptr)delete[] vertex_buffer;
		vertex_buffer = new GLfloat[vbuffer.size()];
	}

	// We now copy the array from our std::vector into memory, "committing" to our structure.
	std::copy(vbuffer.begin(), vbuffer.end(), vertex_buffer);

	// Remember the length of our array to prevent overflows.
	vertex_count = vbuffer.size();

	// Clear the data in our temporary std::vector, we won't need it anymore.
	vbuffer.clear();

	// Now do thesame thing to UVs and tangents if they are present.
	if(has_uvs) {
		if(!keep_size) {
			if(uv_buffer != nullptr)delete[] uv_buffer;
			uv_buffer = new GLfloat[ubuffer.size()];
		}
		std::copy(ubuffer.begin(), ubuffer.end(), uv_buffer);
		uv_count = ubuffer.size();

		if(!keep_size) {
			if(tangent_buffer != nullptr)delete[] tangent_buffer;
			tangent_buffer = new GLfloat[tbuffer.size()];
		}
		std::copy(tbuffer.begin(), tbuffer.end(), tangent_buffer);
		tangent_count = tbuffer.size();
	} else {
		// If we don't have them, indicate it with a nullptr.
		uv_buffer = nullptr;
		tangent_buffer = nullptr;
	}
	ubuffer.clear();
	tbuffer.clear();

	// And now for the normals.
	if(!keep_size) {
		if(normal_buffer != nullptr)delete[] normal_buffer;
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

	// Generate Vertex Array Object (VAO) and give it appropriate values.
	// Look at https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object for
	// more info on VAOs.
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