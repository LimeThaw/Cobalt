#include "name_manager.h"

name_manager* name_manager::instance = nullptr;

name_manager::name_manager() {
	count = 0;
}

name_manager* name_manager::get_instance() {
	if(instance == nullptr) instance = new name_manager();
	return instance;
}

string name_manager::insert(string name, node* item) {
	bool found = false;
	while(dictionary.find(name) != dictionary.end()) {
		found = true;
		name = string("Node_") + to_string(count++);
	}
	if(found) printf("! Two meshes had the same name, one has been renamed\n");
	dictionary.insert(pair<string, node*>(name, item));
	printf(" -> Inserted node %s\n", name.c_str());
	return name;
}

string name_manager::insert(node* item) {
	return insert(string("Node_") + to_string(count++), item);
}
