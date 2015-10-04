#include "node.h"

node::node() {
    location = rotation = scale = node_matrix = glm::mat4(1.0f);
    parent_node = nullptr;
}

node::node(const std::string &scene_path) {
    location = rotation = scale = node_matrix = glm::mat4(1.0f);
    parent_node = nullptr;
    load_scene(scene_path);
}

node::node(const std::string &scene_path, material_id material) {
    location = rotation = scale = node_matrix = glm::mat4(1.0f);
    parent_node = nullptr;
    load_scene(scene_path);
    set_material(material);
}

node::~node() {
    for(unsigned int i = 0; i < models.size(); i++) {
        delete models[i];
    }
    for(unsigned int i = 0; i < children.size(); i++) {
        delete children[i];
    }
    models.clear();
    children.clear();
}

void node::load_model(const std::string &path) {
    mesh *temp_mesh = new mesh();
    temp_mesh->load_model(path);
    models.push_back(temp_mesh);
}

bool node::load_scene(const std::string &path) {
    std::clog << "-Loading scene " << path << "\n\n";
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

    std::clog << "- Finished loading scene with " << scene->mNumMeshes << " meshes in " << glfwGetTime() - start_time << "seconds\n\n";
    return true;
}

void node::set_material(unsigned int new_materialID) {
    for(unsigned int i = 0; i < models.size(); i++) {
        models[i]->set_material(new_materialID);
    }
    for(unsigned int i = 0; i < children.size(); i++) {
        children[i]->set_material(new_materialID);
    }
}

void node::place(float x, float y, float z) {
    location = glm::translate(glm::vec3(x, y, z));
}

void node::set_orientation(float x, float y, float z) {
    glm::quat rot = glm::quat(glm::vec3(x, y , z));
    rotation = glm::mat4_cast(rot);
}

void node::set_scale(float x, float y, float z) {
    scale = glm::scale(glm::vec3(x, y, z));
}

void node::set_scale(float new_scale) {
    scale = glm::scale(glm::vec3(new_scale));
}

void node::append_node(const std::string &file_path) {
    node *temp_node = new node();
    temp_node->load_scene(file_path);
    temp_node->set_parent(this);
    children.push_back(temp_node);
}

void node::append_node(node *new_child) {
    new_child->set_parent(this);
    children.push_back(new_child);
}

void node::set_parent(node *new_parent) {
    if(parent_node != nullptr) {
        parent_node->remove_child(this);
    }
    parent_node = new_parent;
}

bool node::remove_child(node *child) {
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
    temp_matrix = location * rotation * scale;
    if(parent_node != nullptr)temp_matrix = parent_node->get_node_matrix() * temp_matrix;
    return temp_matrix;
}

void node::render(glm::mat4 view_matrix) const {
    for(unsigned int i = 0; i < models.size(); i++) {
        models[i]->render(get_node_matrix(), view_matrix);
    }
    for(unsigned int i = 0; i < children.size(); i++) {
        children[i]->render(view_matrix);
    }
}

void node::render(glm::mat4 parent_matrix, glm::mat4 view_matrix) {
    node_matrix = location * rotation * scale;        //Calculate the node matrix
    glm::mat4 sum_matrix = parent_matrix * node_matrix;

    for(unsigned int i = 0; i < models.size(); i++) {
        models[i]->render(sum_matrix, view_matrix);
    }
    for(unsigned int i = 0; i < children.size(); i++) {
        children[i]->render(sum_matrix, view_matrix);
    }
}

std::vector< node * > node::enumerate() {
    std::vector< node * > rst;
    rst.push_back(this);
    for(auto i : children) {
        auto ie = i->enumerate();
        rst.insert(rst.end(), ie.begin(), ie.end());
    }
    return rst;
}

const std::vector< mesh * > &node::get_models() {
    return models;
}


//Private
void node::load_model(const std::string &path, int model_index) {
    mesh *temp_mesh = new mesh();
    temp_mesh->load_model(path, model_index);
    models.push_back(temp_mesh);
}
