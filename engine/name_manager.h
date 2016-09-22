#include <string>
#include <map>

using namespace std;
class node;

class name_manager {
	public:
		static name_manager* get_instance();
		string insert(string name, node* item);
		string insert(node* item);
	
	private:
		name_manager();
		static name_manager* instance;
		map<string, node*> dictionary;
		int count;
};
