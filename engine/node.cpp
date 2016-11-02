#include "node.h"
#include "mesh.h"

node::node(std::string name) : named(name) {
    node_matrix = glm::mat4(1.0f);
    parent_node = nullptr;
}

node::node(const std::string &scene_path, std::string name) : node(name) {
    load_scene(scene_path);
}

node::~node() {
    children.clear();
}

void node::load_model(const std::string &path) {
    auto temp_mesh = std::make_shared<mesh>(); // TODO: make mesh_ptr
    temp_mesh->load_model(path);
    append_node(static_cast<node_ptr>(temp_mesh));
}

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

void node::set_material(std::shared_ptr<material> new_material) {
    for(unsigned int i = 0; i < children.size(); i++) {
        children[i]->set_material(new_material);
    }
}

void node::place(float x, float y, float z) {
    node_matrix[3][0] = x;
    node_matrix[3][1] = y;
    node_matrix[3][2] = z;
}

void node::place(glm::vec3 arg_position) {
	place(arg_position.x, arg_position.y, arg_position.z);
}

void node::move(float x, float y, float z) {
    move(glm::vec3(x, y, z));
}

void node::move(glm::vec3 arg_movement) {
	node_matrix = glm::translate(arg_movement) * node_matrix;
}

void node::move_relative(float x, float y, float z) {
	glm::vec3 tmp(get_node_matrix() * glm::vec4(x, y, z, 0));
	move(tmp);
}

void node::rotate(float x, float y, float z) {
	node_matrix = node_matrix * glm::mat4_cast(glm::quat(glm::vec3(x, y, z)));
}

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

void node::set_scale(float new_scale) {
	set_scale(new_scale, new_scale, new_scale);
}

void node::look_at(float x, float y, float z, glm::vec3 up_vector) {
	look_at(glm::vec3(x, y, z), up_vector);
}

void node::look_at(glm::vec3 arg_look, glm::vec3 up_vector) {
	glm::vec3 current_position(node_matrix[3][0], node_matrix[3][1], node_matrix[3][2]);
	node_matrix = glm::inverse(glm::lookAt(current_position, arg_look, up_vector));
}

void node::append_node(const std::string &file_path) {
    node_ptr temp_node;
    temp_node->load_scene(file_path);
    append_node(temp_node);
}

void node::append_node(node_ptr new_child) {
	try {
    	new_child->set_parent(shared_from_this());
	    children.push_back(new_child);
	} catch(std::bad_weak_ptr) {
		printf("%s! Error: Could not append node %s to node %s - please use the pointer wrappers.",
			indent::get().c_str(), new_child->get_name().c_str(), get_name().c_str());
	}
}

void node::append_mesh(const std::string &file_path) {
	auto temp_mesh = make_shared<mesh>(); // TODO: make mesh_ptr
	temp_mesh->load_model(file_path);
	append_node(node_ptr(temp_mesh));
}

void node::set_parent(node_ptr new_parent) {
    if(parent_node != nullptr) {
		try {
        	parent_node->remove_child(shared_from_this());
		} catch(std::bad_weak_ptr) {
			printf("%s! Error: Failed to update parent of node %s: Please use the pointer wrappers.",
				indent::get().c_str(), get_name().c_str());
		}
    }
    parent_node = new_parent;
}

node_ptr node::get_parent() {
	return parent_node;
}

bool node::remove_child(node_ptr child) {
    for(unsigned int i = 0; i < children.size(); i++) {
        if(children[i] == child) {
            children.erase(children.begin() + i);
            return true;
        }
    }
    return false;
}

glm::mat4 node::get_node_matrix() const {
    glm::mat4 temp_matrix;
    temp_matrix = node_matrix;
    if(parent_node != nullptr)temp_matrix = parent_node->get_node_matrix() * temp_matrix;
    return temp_matrix;
}

glm::mat4 node::get_isolated_matrix() const {
	return node_matrix;
}

void node::set_node_matrix(glm::mat4 matrix) {
	node_matrix = matrix;
}

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

std::vector<node_ptr> node::get_children() {
	return children;
}

//Private
void node::load_model(const std::string &path, int model_index) {
    auto temp_mesh = make_shared<mesh>(); // TODO: Make mesh_ptr
    temp_mesh->load_model(path, model_index);
    append_node(static_cast<node_ptr>(temp_mesh));
}
