#include "util.h"

string serialize(glm::vec3 vec) {
	string s = to_string(vec.x);
	s.append(" ");
	s.append(to_string(vec.y));
	s.append(" ");
	s.append(to_string(vec.z));
	return s;
}

glm::vec3 deserialize_vec3(string text) {
	glm::vec3 ret;
	auto words = split(text, ' ');
	assert(words.size() >= 3);

	for(short i = 0; i < 3; ++i) {
		ret[i] = stof(words[i]);
	}
	return ret;
}

string serialize(glm::mat4 mat) {
	string s = to_string(mat[0][0]);
	int j = 1;
	for(int i = 0; i < 4; ++i) {
		for( ; j < 4; ++j) {
			s += " " + to_string(mat[i][j]);
		}
		j = 0;
	}
	return s;
}

glm::mat4 deserialize_mat4(string text) {
	glm::mat4 ret;
	auto words = split(text, ' ');
	assert(words.size() >= 16);

	for(short i = 0; i < 4; ++i) {
		for(short j = 0; j < 4; ++j) {
			ret[i][j] = stof(words[4*i + j]);
		}
	}
	return ret;
}

void write_to_file(const char* filename, const char* text) {
	std::ofstream file(filename);
	if(file.fail()) {
		printf("! Failed to write to file %s\n", filename);
		return;
	}
	file << text;
	file.close();
}

string read_from_file(const char* filename) {
	std::ifstream file(filename);
	if(file.fail()) {
		printf("! faled to open file %s for reading\n", filename);
		return "";
	}
	string text, line;
	while(!file.eof()) {
		getline(file, line);
		text += line + "\n";
	}
	return text;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

string filename(string source) {
	auto list = split(source, '/');
	return list[list.size() - 1];
}