#include "util.h"

string serialize(glm::vec3 vec) {
	string s = to_string(vec.x);
	s.append(", ");
	s.append(to_string(vec.y));
	s.append(", ");
	s.append(to_string(vec.z));
	return s;
}

string serialize(glm::mat4 mat) {
	string s = to_string(mat[0][0]);
	int j = 1;
	for(int i = 0; i < 4; ++i) {
		for( ; j < 4; ++j) {
			s += ", " + to_string(mat[i][j]);
		}
		j = 0;
	}
	return s;
}

void write_to_file(const char* filename, const char* text) {
	std::ofstream file(filename);
	if(file.fail()) {
		printf("! Failed to write to file %s\n", filename);
	}
	file << text;
	file.close();
}
