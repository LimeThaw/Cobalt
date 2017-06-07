#include "node.h"
#include "mesh.h"

/**
 Default constructor.
*/
node::node(std::string name) : named(name) {
    node_matrix = glm::mat4(1.0f);
    parent_node = weak_ptr<node>();
}

/** Constructor loading all meshes from a file and appending them to the node. */
node::node(const std::string &scene_path, std::string name) : node(name) {
    load_scene(scene_path);
}

/** Default destructor. */
node::~node() {
    children.clear();
}

/** Loads the first mesh in the specified file and appends it to the node. */
void node::load_model(const std::string &path) {
    mesh_ptr temp_mesh;
    temp_mesh->load_model(path);
    append_node(node_ptr(temp_mesh));
}

/** Loads all meshes from the specified file and appends them to the node. */
bool node::load_scene(const std::string &path) {
    std::clog << "- Loading scene " << path << "\n";
    float start_time = glfwGetTime();

    Assimp::Importer importer;      //Create importer object
    const aiScene *scene = importer.ReadFile(path,
                           aiProcess_CalcTangentSpace |
                           aiProcess_Triangulate |
                           aiProcess_JoinIdenticalVertices |
                           aiProcess_SortByPType |
                           aiProcess_GenSmoothNormals);    //Load scene

    if(scene == nullptr) {
        return false;
    }

    for(unsigned int i = 0; i < scene->mNumMeshes; i++) {
        load_model(path, i);
    }

    std::clog << " - Finished loading scene with " << scene->mNumMeshes << " meshes in " << glfwGetTime() - start_time << " seconds\n\n";
    return true;
}

/** Sets the material for all nodes and meshes appended to this node. */
void node::set_material(std::shared_ptr<material> new_material) {
    for(unsigned int i = 0; i < children.size(); i++) {
        children[i]->set_material(new_material);
    }
}

/** Specifies the node location relative to its parent node. */
void node::place(float x, float y, float z) {
    node_matrix[3][0] = x;
    node_matrix[3][1] = y;
    node_matrix[3][2] = z;
}

/** See member place(float x, float y, float z). */
void node::place(glm::vec3 arg_position) {
	place(arg_position.x, arg_position.y, arg_position.z);
}

/** Moves the node relative to its parent node. */
void node::move(float x, float y, float z) {
    move(glm::vec3(x, y, z));
}

/** See member move(float x, float y, float z). */
void node::move(glm::vec3 arg_movement) {
	node_matrix = glm::translate(arg_movement) * node_matrix;
}

/** Moves the node relative to its parent considering the node's rotation. */
void node::move_relative(float x, float y, float z) {
	glm::vec3 tmp(get_node_matrix() * glm::vec4(x, y, z, 0));
	move(tmp);
}

/** Rotates the node for the given angles around the respective axes. */
void node::rotate(float x, float y, float z) {
	node_matrix = node_matrix * glm::mat4_cast(glm::quat(glm::vec3(x, y, z)));
}

/** Sets the node's scale. */
void node::set_scale(float x, float y, float z) {
	if(x < 0) x = -x;
	if(y < 0) y = -y;
	if(z < 0) z = -z;
	glm::vec3 current_scale(glm::length(glm::vec3(node_matrix[0][0], node_matrix[1][0],
		node_matrix[2][0])), glm::length(glm::vec3(node_matrix[0][1], node_matrix[1][1],
		node_matrix[2][1])), glm::length(glm::vec3(node_matrix[0][2], node_matrix[1][2],
		node_matrix[2][2])));
    node_matrix = node_matrix * glm::inverse(glm::scale(current_scale));
    node_matrix = node_matrix * glm::scale(glm::vec3(x, y, z));
}

/** See member set_scale(float x, float y, float z). */
void node::set_scale(float new_scale) {
	set_scale(new_scale, new_scale, new_scale);
}

/** Rotates the node to face the specified point in space. */
void node::look_at(float x, float y, float z, glm::vec3 up_vector) {
	look_at(glm::vec3(x, y, z), up_vector);
}

/** See member look_at(float x, float y, float z). */
void node::look_at(glm::vec3 arg_look, glm::vec3 up_vector) {
	glm::vec3 current_position(node_matrix[3][0], node_matrix[3][1], node_matrix[3][2]);
	node_matrix = glm::inverse(glm::lookAt(current_position, arg_look, up_vector));
}

/** Basically append_node(new node(file_path)). */
void node::append_node(const std::string &file_path) {
    node_ptr temp_node;
    temp_node->load_scene(file_path);
    append_node(temp_node);
}

/** Appends the given node as its child. */
void node::append_node(node_ptr new_child) {
	try {
    	new_child->set_parent(shared_from_this());
	    children.push_back(new_child);
	} catch(std::bad_weak_ptr) {
		printf("%s! Error: Could not append node %s to node %s - please use the pointer wrappers.",
			indent::get().c_str(), new_child->get_name().c_str(), get_name().c_str());
	}
}

/** Loads the first mesh from the given path and appends it as its child node */
void node::append_mesh(const std::string &file_path) {
	mesh_ptr temp_mesh;
	temp_mesh->load_model(file_path);
	append_node(node_ptr(temp_mesh));
}

/** Set the node's parent. */
void node::set_parent(node_ptr new_parent) {
	shared_ptr<node> pn;
    if(!parent_node.expired() && (pn = parent_node.lock()) != nullptr) {
		try {
        	pn->remove_child(shared_from_this());
		} catch(std::bad_weak_ptr) {
			printf("%s! Error: Failed to update parent of node %s: Please use the pointer wrappers.",
				indent::get().c_str(), get_name().c_str());
		}
    }
    parent_node = new_parent;
}


/** Returns a node_ptr to its parent node */
node_ptr node::get_parent() {
	return parent_node.lock();
}

/** Removes a child node from this node. */
bool node::remove_child(node_ptr child) {
    for(unsigned int i = 0; i < children.size(); i++) {
        if(children[i] == child) {
            children.erase(children.begin() + i);
            return true;
        }
    }
    return false;
}
/**
 *  Returns the transformation matrix affecting all of the node's children. It includes the
 *  transformation of it's parents.
 */
glm::mat4 node::get_node_matrix() const {
    glm::mat4 temp_matrix;
    temp_matrix = node_matrix;
    if(!parent_node.expired() && parent_node.lock() != nullptr)temp_matrix = parent_node.lock()->get_node_matrix() * temp_matrix;
    return temp_matrix;
}

/** Returns the matrix describing the transformation of this node relative to its parent. */
glm::mat4 node::get_isolated_matrix() const {
	return node_matrix;
}

/** Sets the matrix describing the node's relative transformation. */
void node::set_node_matrix(glm::mat4 matrix) {
	node_matrix = matrix;
}

/** Returns a list of itself and all its child nodes. */
std::vector<node_ptr> node::enumerate() {
    std::vector<node_ptr> rst;
	try {
    	rst.push_back(shared_from_this());
	} catch(std::bad_weak_ptr) {
		printf("%s! Error: Failed to enumerate node %s: Please use pointer wrappers.",
			indent::get().c_str(), get_name().c_str());
	}
    for(auto i : children) {
        auto ie = i->enumerate();
        rst.insert(rst.end(), ie.begin(), ie.end());
    }
    return rst;
}

/** Returns a list of all direct children of this node */
std::vector<node_ptr> node::get_children() {
	return children;
}

//Private
void node::load_model(const std::string &path, int model_index) {
    mesh_ptr temp_mesh;
    temp_mesh->load_model(path, model_index);
    append_node(node_ptr(temp_mesh));
}
