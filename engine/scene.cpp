#include "scene.h"

scene::scene() {
    init();
}

scene::~scene() {
    //dtor
}

void scene::append_node(const std::string &file_path) {
    parent_node.append_node(file_path);
}

void scene::append_node(node *new_child) {
    parent_node.append_node(new_child);
}

node *scene::get_parent_node() {
    return &parent_node;
}

std::vector< node * > scene::enumerate_nodes() {
    return parent_node.enumerate();
}

//private
void scene::init() {

}