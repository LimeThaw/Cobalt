#include "name_manager.h"

name_manager* name_manager::instance = nullptr; ///< Initializing the singleton instance

/**
 *  The default constructor. Simply initalizes the state reasonably.
 */
name_manager::name_manager() {
	count = 0;
}

/**
 *  Returns the singleton instance of the class.
 */
name_manager* name_manager::get_instance() {
	if(instance == nullptr) instance = new name_manager();
	return instance;
}

/**
 *  Inserts a name into the manager's record. If the name is not take yet it simply inserts and returns
 *  the given name. Otherwise it will create a new name (in the form Item_X with X as an integer) and
 *  insert and return that name instead. So the name that your item is now registered under is the one
 *  returned by this method.
 *  @param	name	The name you want to register your object under.
 *  @param	item	A pointer to the object you want to register, so we can find it later using
 *  					the name.
 */
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

/**
 *  Inserts an item with generated name. Works similar to the other insert, just that it generates
 *  an "Item_X"-name right away instead of trying your suggestion (since you did't give one).
 *  @param	item	A pointer to the object that needs a name.
 */
string name_manager::insert(void* item) {
	return insert(string("Item_") + to_string(count++), item);
}

/**
 *  Checks whether the given name is assigned to an object.
 *  @param	name	Welcome to the name_manager. Who are you here to see?
 */
bool name_manager::contains(string name) {
	return dictionary.find(name) != dictionary.end();
}

/**
 *  Removes the entry for the given name if it exists.
 *  @param	name	Who do you want killed? Are you sure?
 */
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
