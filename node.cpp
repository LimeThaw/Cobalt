#include "node.h"

node::node() {
    location = rotation = scale = node_matrix = glm::mat4(1.0f);
    parent_node = NULL;
}

node::node(const char *scene_path) {
    location = rotation = scale = node_matrix = glm::mat4(1.0f);
    parent_node = NULL;
    load_scene(scene_path);
}

node::~node() {
    for(int i = 0; i < models.size(); i++) {
        delete models[i];
    }
    for(int i = 0; i < children.size(); i++) {
        delete children[i];
    }
    models.clear();
    children.clear();
}

void node::load_model(const char *path) {
    mesh *temp_mesh = new mesh();
    temp_mesh->load_model(path);
    models.push_back(temp_mesh);
}

bool node::load_scene(const char *path) {
    std::clog << "-Loading scene " << path << "\n\n";
    float start_time = glfwGetTime();

    Assimp::Importer importer;      //Create importer object
    const aiScene *scene = importer.ReadFile(path,
                           aiProcess_CalcTangentSpace |
                           aiProcess_Triangulate |
                           aiProcess_JoinIdenticalVertices |
                           aiProcess_SortByPType |
                           aiProcess_GenSmoothNormals);    //Load scene

    if(scene == NULL) {
        return false;
    }

    for(int i = 0; i < scene->mNumMeshes; i++) {
        load_model(path, i);
    }

    std::clog << "- Finished loading scene with " << scene->mNumMeshes << " meshes in " << glfwGetTime() - start_time << "seconds\n\n";
    return true;
}

void node::set_material(unsigned int new_materialID) {
    for(int i = 0; i < models.size(); i++) {
        models[i]->set_material(new_materialID);
    }
    for(int i = 0; i < children.size(); i++) {
        children[i]->set_material(new_materialID);
    }
}

void node::place(float x, float y, float z) {
    location = glm::translate(glm::vec3(x, y, z));
}

void node::set_orientation(float x, float y, float z) {
    glm::quat rot = glm::quat(glm::vec3(x * 0.017453294, y * 0.017453294, z * 0.017453294));
    rotation = glm::mat4_cast(rot);
}

void node::set_scale(float x, float y, float z) {
    scale = glm::scale(glm::vec3(x, y, z));
}

void node::set_scale(float new_scale) {
    scale = glm::scale(glm::vec3(new_scale));
}

void node::append_node(const char *file_path) {
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
    if(parent_node != NULL) {
        parent_node->remove_child(this);
    }
    parent_node = new_parent;
}

bool node::remove_child(node *child) {
    for(int i = 0; i < children.size(); i++) {
        if(children[i] == child) {
            children.erase(children.begin() + i);
            return true;
        }
    }
    return false;
}

glm::mat4 node::get_node_matrix() {
    glm::mat4 temp_matrix;
    temp_matrix = location * rotation * scale;
    if(parent_node != NULL)temp_matrix = parent_node->get_node_matrix() * temp_matrix;
    return temp_matrix;
}

glm::mat4 node::get_rotation_matrix() {
    if(parent_node != NULL) {
        return parent_node->get_rotation_matrix() * rotation;
    } else {
        return rotation;
    }
}

void node::render() {
    for(int i = 0; i < models.size(); i++) {
        models[i]->render(get_node_matrix(), get_rotation_matrix());
    }
    for(int i = 0; i < children.size(); i++) {
        children[i]->render();
    }
}

void node::render(glm::mat4 parent_matrix, glm::mat4 parent_rotation_matrix) {
    node_matrix = location * rotation * scale;        //Calculate the node matrix
    glm::mat4 sum_matrix = parent_matrix * node_matrix;
    glm::mat4 sum_rotation_matrix = parent_rotation_matrix * rotation;

    for(int i = 0; i < models.size(); i++) {
        models[i]->render(sum_matrix, sum_rotation_matrix);
    }
    for(int i = 0; i < children.size(); i++) {
        children[i]->render(sum_matrix, sum_rotation_matrix);
    }
}

//Private
bool node::load_model(const char *path, int model_index) {
    mesh *temp_mesh = new mesh();
    temp_mesh->load_model(path, model_index);
    models.push_back(temp_mesh);
}
