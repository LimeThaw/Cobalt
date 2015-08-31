//Class used to refer to OpenGL programs for rendering
//All shader objects should be handled by the shader_manager

#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class shader {
    public:
        shader();
        ~shader();
        void load_shader(const std::string &vertex_path, const std::string &fragment_path);
        void use();
        GLuint get_shader_id();
        unsigned int get_instance_count();
        void add_instance();
        void remove_instance();

    private:
        GLuint shader_id;
        unsigned int instances;
};

const int shader_vertex_location = 0;
const int shader_uv_location = 1;
const int shader_normal_location = 2;
const int shader_tangent_location = 3;

#endif // SHADER_H
