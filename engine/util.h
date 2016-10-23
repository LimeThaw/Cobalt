#ifndef UTIL_H
#define UTIL_H

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <glm/glm.hpp>
#include <cassert>
#include <cmath>

#define DEG_TO_RAD 0.017453294

using namespace std;

template<typename T>
T clamp(const T &val, const T &min, const T &max) {
    if(val > max) {
        return max;
    } else if(val < min) {
        return min;
    } else {
        return val;
    }
}

inline unsigned char normalized_float_to_byte(float x) {
    assert(0 <= x && x <= 1);
    return (unsigned char) (x * 255);
}

template<typename T>
T log2(T x) {
    return std::log(x) / std::log(2);
}

string serialize(glm::vec3 vec);
glm::vec3 deserialize_vec3(string text);
string serialize(glm::mat4 mat);
glm::mat4 deserialize_mat4(string text);

void write_to_file(const char* filename, const char* text);
string read_from_file(const char* filename);

vector<string> split(const string &s, char delim);
string filename(string source);

#endif
