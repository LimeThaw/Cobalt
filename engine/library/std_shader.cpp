#include "std_shader.h"

std_shader::std_shader() : actual_shader(shader("std_shader.vertex","std_shader.fragment")) {
	
}

void std_shader::use() {
	actual_shader.use();
}
