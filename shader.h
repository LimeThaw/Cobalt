#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class shader{
    public:
        shader();
        ~shader();
        void load_shader(const char* vertex_path, const char* fragment_path);
        void use();
        GLuint get_shaderID();
        unsigned int get_instance_count();
        void add_instance();
        void remove_instance();

    private:
        GLuint shaderID;
        unsigned int instances;
};

#endif // SHADER_H
