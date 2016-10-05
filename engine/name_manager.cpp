#include "name_manager.h"

name_manager* name_manager::instance = nullptr;

name_manager::name_manager() {
	count = 0;
}

name_manager* name_manager::get_instance() {
	if(instance == nullptr) instance = new name_manager();
	return instance;
}

string name_manager::insert(string name, void* item) {
	if(name == "") return insert(item);

	bool found = false;
	while(dictionary.find(name) != dictionary.end()) {
		found = true;
		name = string("Item_") + to_string(count++);
	}
	if(found) printf("! Two Items had the same name, one has been renamed\n");
	dictionary.insert(pair<string, void*>(name, item));
	printf(" -> Inserted item %s\n", name.c_str());
	return name;
}

string name_manager::insert(void* item) {
	return insert(string("Item_") + to_string(count++), item);
}

bool name_manager::contains(string name) {
	return dictionary.find(name) != dictionary.end();
}

bool name_manager::remove(string name) {
	if(!contains(name)) {
		printf("! Could not remove %s: Element does not exist\n", name.c_str());
		return false;
	} else {
		dictionary.erase(name);
		printf(" -> Removed %s\n", name.c_str());
		return true;
	}
}