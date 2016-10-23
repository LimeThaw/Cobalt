#include "shader.h"

static GLuint load_shader(const std::string &vertex_path, const std::string &fragment_path, const std::string shader_prefix) {
    std::clog << "- Loading shader " << filename(vertex_path) << " | " << filename(fragment_path) << "\n";
    float start_time = glfwGetTime();

    // Create the shaders
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string vertex_shader_code = shader_prefix + "\n";
    std::ifstream vertex_shader_stream(vertex_path, std::ios::in);
    if(vertex_shader_stream.is_open()) {
        std::string line = "";
        while(getline(vertex_shader_stream, line))
            vertex_shader_code += "\n" + line;
        vertex_shader_stream.close();
    }
    vertex_shader_code = shader::process_shader(vertex_shader_code);

    // Read the Fragment Shader code from the file
    std::string fragment_shader_source = shader_prefix + "\n";
    std::ifstream fragment_shader_stream(fragment_path, std::ios::in);
    if(fragment_shader_stream.is_open()) {
        std::string line = "";
        while(getline(fragment_shader_stream, line))
            fragment_shader_source += "\n" + line;
        fragment_shader_stream.close();
    }
    fragment_shader_source = shader::process_shader(fragment_shader_source);

    GLint result = GL_FALSE;
    int info_log_length;

    // Compile Vertex Shader
    if(DEBUG_INFO) printf(" - Compiling shader : %s\n", vertex_path.c_str());
    char const *vertex_source_counter = vertex_shader_code.c_str();
    glShaderSource(vertex_shader_id, 1, &vertex_source_counter , NULL);
    glCompileShader(vertex_shader_id);

    // Check Vertex Shader
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
    if(result != GL_TRUE) {
        glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> vertex_shader_error_message(info_log_length);
        glGetShaderInfoLog(vertex_shader_id, info_log_length, NULL, &vertex_shader_error_message[0]);
        fprintf(stdout, " -  %s\n", &vertex_shader_error_message[0]);
    }

    // Compile Fragment Shader
    if(DEBUG_INFO) printf(" - Compiling shader : %s\n", fragment_path.c_str());
    char const *fragment_source_pointer = fragment_shader_source.c_str();
    glShaderSource(fragment_shader_id, 1, &fragment_source_pointer , NULL);
    glCompileShader(fragment_shader_id);

    // Check Fragment Shader
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
    if(result != GL_TRUE) {
        glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> fragment_shader_error_message(info_log_length);
        glGetShaderInfoLog(fragment_shader_id, info_log_length, NULL, &fragment_shader_error_message[0]);
        fprintf(stdout, " -  %s\n", &fragment_shader_error_message[0]);
    }

    // Link the program
    if(DEBUG_INFO) fprintf(stdout, " - Linking program\n");
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glBindAttribLocation(program_id, shader_vertex_location, "vertex_position");
    glBindAttribLocation(program_id, shader_uv_location, "vertex_UV");
    glBindAttribLocation(program_id, shader_normal_location, "vertex_normal");
    glBindAttribLocation(program_id, shader_tangent_location, "vertex_tangent");
    glLinkProgram(program_id);

    // Check the program
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    if(result != GL_TRUE) {
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> program_error_message(std::max(info_log_length, int(1)));
        glGetProgramInfoLog(program_id, info_log_length, NULL, &program_error_message[0]);
        fprintf(stdout, " %s\n", &program_error_message[0]);
    }

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    GLuint shader_id = program_id;

    if(DEBUG_INFO) std::clog << " - Finished loading shader " << shader_id << " in " << glfwGetTime() - start_time << " seconds\n\n";

    return shader_id;
}

shader::shader(const std::string &vertex_path, const std::string &fragment_path, const std::string shader_prefix) {
    shader_id = load_shader(vertex_path, fragment_path, shader_prefix);
}

shader::~shader() {
    glDeleteProgram(shader_id);
}

void shader::use() {
    glUseProgram(shader_id);
}

std::string shader::process_shader(std::string source) {

	std::string term, value;

	// Substitute defines
	unsigned int a = 0, b, c, length;
	while(source.find("#define", a) < source.length()) {
		a = source.find("#define", a) + 7;
		b = source.find_first_not_of(" ", a);
		c = source.find_first_of(" ", b);
		length = c - b;
		term = source.substr(b, length);

		b = source.find_first_not_of(" ", c);
		c = std::min(std::min(source.find_first_of(" ", b), source.find_first_of("	", b)), source.find_first_of("\n", b));
		value = source.substr(b, c - b);

		while(source.find(term, c) < source.length()) {
			b = source.find(term, c);
			source = source.substr(0, b) + value + source.substr(b + length);
		}
	}

	// Process commands
	unsigned int d, e;
	std::string command, data, result;
	while(source.find("::") < source.length()) {
		result = "";
		a = source.find("::");
		b = source.find("--", a);
		if(b < source.length()) {
			c = source.find(";;", b);
			if(c > source.length()) break;
		} else break;

		command = source.substr(a+2, b-a-2);
		data = source.substr(b+2, c-b-2);

		if(command.substr(0, 3) == "for") {
			b = command.find_first_not_of(" ", 3);
			d = command.find_first_of(" ", b);
			term = command.substr(b, d - b);
			b = command.find_first_not_of(" ", d);
			d = std::min(command.find_first_of(" ", b), command.find_first_of("-", b));
			value = command.substr(b, d - b);
			std::stringstream(value) >> d;

			for(unsigned int i = 0; i < d; ++i) {
				result += data;
				while(result.find(" " + term + " ") < result.length()) {
					b = result.find(" " + term + " ");
					e = b + value.length() + 2;
					result = result.substr(0, b) + std::to_string(i) + result.substr(e);
				}
			}
		}

		source = source.substr(0, a) + result + source.substr(c + 2);
	}

	// ::for i 5--print(i);;  ==  for(int i = 0; i < 5, ++i) print(i);

	return source;
}
