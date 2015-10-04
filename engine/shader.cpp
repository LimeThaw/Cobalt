#include "shader.h"

shader::shader() {
    shader_id = 0;
    instances = 0;
}

shader::~shader() {
    glDeleteProgram(shader_id);
}

void shader::load_shader(const std::string &vertex_path, const std::string &fragment_path) {
    std::clog << "-Loading shader\n";
    float start_time = glfwGetTime();

    // Create the shaders
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string vertex_shader_code;
    std::ifstream vertex_shader_stream(vertex_path, std::ios::in);
    if(vertex_shader_stream.is_open()) {
        std::string line = "";
        while(getline(vertex_shader_stream, line))
            vertex_shader_code += "\n" + line;
        vertex_shader_stream.close();
    }

    // Read the Fragment Shader code from the file
    std::string fragment_shader_source;
    std::ifstream fragment_shader_stream(fragment_path, std::ios::in);
    if(fragment_shader_stream.is_open()) {
        std::string line = "";
        while(getline(fragment_shader_stream, line))
            fragment_shader_source += "\n" + line;
        fragment_shader_stream.close();
    }

    GLint result = GL_FALSE;
    int info_log_length;

    // Compile Vertex Shader
    printf("- Compiling shader : %s\n", vertex_path.c_str());
    char const *vertex_source_counter = vertex_shader_code.c_str();
    glShaderSource(vertex_shader_id, 1, &vertex_source_counter , NULL);
    glCompileShader(vertex_shader_id);

    // Check Vertex Shader
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
    if(result != GL_TRUE) {
        glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> vertex_shader_error_message(info_log_length);
        glGetShaderInfoLog(vertex_shader_id, info_log_length, NULL, &vertex_shader_error_message[0]);
        fprintf(stdout, "-  %s\n", &vertex_shader_error_message[0]);
    }

    // Compile Fragment Shader
    printf("- Compiling shader : %s\n", fragment_path.c_str());
    char const *fragment_source_pointer = fragment_shader_source.c_str();
    glShaderSource(fragment_shader_id, 1, &fragment_source_pointer , NULL);
    glCompileShader(fragment_shader_id);

    // Check Fragment Shader
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
    if(result != GL_TRUE) {
        glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> fragment_shader_error_message(info_log_length);
        glGetShaderInfoLog(fragment_shader_id, info_log_length, NULL, &fragment_shader_error_message[0]);
        fprintf(stdout, "-  %s\n", &fragment_shader_error_message[0]);
    }

    // Link the program
    fprintf(stdout, "- Linking program\n");
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
        fprintf(stdout, "%s\n", &program_error_message[0]);
    }

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    shader_id = program_id;

    std::clog << "- Finished loading shader in " << glfwGetTime() - start_time << " seconds\n\n";
}

void shader::use() {
    glUseProgram(shader_id);
    /*GLint sampler = glGetUniformLocation(shader_id, "sampler");
    glUniform1i(sampler, 0);*/
}

GLuint shader::get_shader_id() {
    return shader_id;
}

unsigned int shader::get_instance_count() {
    return instances;
}

void shader::add_instance() {
    instances++;
}

void shader::remove_instance() {
    if(instances > 0)instances--;
}
