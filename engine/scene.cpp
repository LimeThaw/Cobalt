#include "scene.h"

/**
 *  Class constructor. Creates an empty parent node that can be appended to.
 */
scene::scene() {
    init();
	parent_node.make();
}

/**
 *  Class destructor.
 */
scene::~scene() {
    //dtor
}

/**
 *  Loads all meshes from a file and appends them to the scene. The mesh nodes will be appended
 *  to a new node, which will be appended to the scene's parent node. Also see
 *  node::append_node(const std::string &file_path).
 *  @param	file_path	The path to the file that contains the mesh(es)
 */
void scene::append_node(const std::string &file_path) {
    parent_node->append_node(file_path);
}

/**
 *  Appends the node behind the given pointer to the parent node. Wrapper for
 *  node::append_node(node_ptr new_child).
 *  @param	new_child	A node_ptr pointing to the node you want inside your scene.
 */
void scene::append_node(node_ptr new_child) {
    parent_node->append_node(new_child);
}

/**
 *  Appends a single mesh from the given file to the parent node. Who wants to guess? CORRECT:
 *  This is a wrapper for node::append_mesh(const std::string &file_path).
 */
void scene::append_mesh(const std::string &file_path) {
	parent_node->append_mesh(file_path);
}

/**
 *  Returns a pointer to the parent node.
 */
node_ptr scene::get_parent_node() {
    return parent_node;
}

/**
 *  Lists all nodes in the scene. Simply calls node::enumerate().
 */
std::vector<node_ptr> scene::enumerate_nodes() {
    return parent_node->enumerate();
}

/**
 *  Finds and returns a node in the scene by name. If there is no node with the given name, the
 *  function will return a nullptr.
 *  @param	name	The name of the node you want to find.
 */
node_ptr scene::find_node(string name) {
	for(node_ptr n : enumerate_nodes()) {
		if(n->get_name().compare(name) == 0) return n;
	}
	return nullptr;
}

//private
void scene::init() {

}
