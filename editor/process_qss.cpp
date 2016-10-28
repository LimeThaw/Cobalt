#include <string>
#include <map>
using namespace std;

string process_qss(map<string, string> vars, string style) {
	size_t pos;
	map<string, string>::iterator v;
	for(v = vars.begin(); v!= vars.end(); ++v) {
		while((pos = style.find("@"+v->first)) != string::npos) {
			style = style.substr(0, pos) + v->second + style.substr(pos+v->first.size()+1);
		}
	}

	return style;
}