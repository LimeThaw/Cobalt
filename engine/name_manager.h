#ifndef NAME_MANAGER_H
#define NAME_MANAGER_H

#ifndef DEBUG_INFO
#define DEBUG_INFO false
#endif

#include <string>
#include <map>

using namespace std;
class node;

class name_manager {
	public:
		static name_manager* get_instance();
		string insert(string name, void* item);
		string insert(void* item);
		bool contains(string name);
		bool remove(string name);

	private:
		name_manager();
		static name_manager* instance;
		map<string, void*> dictionary;
		unsigned long long count;
};

#endif
