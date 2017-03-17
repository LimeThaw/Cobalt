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
	if(name == "") name = string("Item_") + to_string(count++);

	bool found = false;
	while(dictionary.find(name) != dictionary.end()) {
		found = true;
		name = string("Item_") + to_string(count++);
	}
	if(found) printf("%s! Two Items had the same name, one has been renamed\n", indent::get().c_str());
	dictionary.insert(pair<string, void*>(name, item));
	if(DEBUG_INFO) printf("%s-> Inserted item %s\n", indent::get().c_str(), name.c_str());
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
		printf("%s! Could not remove %s: Element does not exist\n", indent::get().c_str(), name.c_str());
		return false;
	} else {
		dictionary.erase(name);
		if(DEBUG_INFO) printf("%s-> Removed %s\n", indent::get().c_str(), name.c_str());
		return true;
	}
}
