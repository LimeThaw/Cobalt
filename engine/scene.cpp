#include "scene.h"

scene::scene() {
    init();
	parent_node.make();
}

scene::~scene() {
    //dtor
}

void scene::append_node(const std::string &file_path) {
    parent_node->append_node(file_path);
}

void scene::append_node(node_ptr new_child) {
    parent_node->append_node(new_child);
}

void scene::append_mesh(const std::string &file_path) {
	parent_node->append_mesh(file_path);
}

node_ptr scene::get_parent_node() {
    return parent_node;
}

std::vector<node_ptr> scene::enumerate_nodes() {
    return parent_node->enumerate();
}

node_ptr scene::find_node(string name) {
	for(node_ptr n : enumerate_nodes()) {
		if(n->get_name().compare(name) == 0) return n;
	}
	return nullptr;
}

//private
void scene::init() {

}
