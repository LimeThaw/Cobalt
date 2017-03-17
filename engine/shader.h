//Class used to refer to OpenGL programs for rendering
//All shader objects should be handled by the shader_manager

#ifndef SHADER_H
#define SHADER_H

#ifndef DEBUG_INFO
#define DEBUG_INFO false
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "util.h"
#include "indent.h"
#include "pointer_wrapper.h"

class shader {
    public:
        shader(const std::string &vertex_path, const std::string &fragment_path, const std::string shader_prefix = "");
        ~shader();
        void use();
        static std::string process_shader(std::string filename);
    protected:
        GLuint shader_id;
};

const int shader_vertex_location = 0;
const int shader_uv_location = 1;
const int shader_normal_location = 2;
const int shader_tangent_location = 3;

typedef pointer_wrapper<shader> shader_ptr;

#endif // SHADER_H
