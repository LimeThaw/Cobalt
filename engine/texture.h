//Class used to refer to an OpenGL texture for rendering
//All texture objects should be handled by the texture_manager

#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <string>
#include "texture_data_source.h"
#include "name_manager.h"
#include "gl_exception.h"
#include <glm/glm.hpp>

class texture {

        friend class texture_framebuffer_attachment;

    private:
        GLuint openGL_id;
    protected:
    	string name;
    	string path;
        GLuint get_openGL_id();
    public:
        texture(std::string arg_name = "");
        std::string set_name(const std::string arg_name);
        std::string get_name();
        std::string get_path();

        virtual ~texture();

        virtual void bind() = 0;
};

#endif // TEXTURE_H
