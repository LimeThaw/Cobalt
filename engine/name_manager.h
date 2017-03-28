#ifndef NAME_MANAGER_H
#define NAME_MANAGER_H

#ifndef DEBUG_INFO
#define DEBUG_INFO false
#endif

#include <string>
#include <map>

#include "indent.h"

using namespace std;
class node;

/**
 *  The manager of names. This is a singleton class with the primary purpose of making sure that
 *  no two objects of a scene can have the same name, as their names are used to uniquely
 *  identify and find them.
 */
class name_manager {
	public:
		static name_manager* get_instance();
		string insert(string name, void* item);
		string insert(void* item);
		bool contains(string name);
		bool remove(string name);

	private:
		name_manager();
		static name_manager* instance; ///< The singleton instance
		map<string, void*> dictionary; ///< The names currently in use
		unsigned long long count; ///< A counter used to generate new names if necessary
};

#endif
