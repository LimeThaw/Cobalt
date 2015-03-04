#include "texture.h"
#include "gl_exception.h"

texture::texture() {
    openGL_id = 0;
}

texture::~texture() {
    glDeleteTextures(1, &openGL_id);
}

bool texture::load(const char *path) {
    std::clog << "-Loading texture " << path << '\n';
    float start_time = glfwGetTime();

    // Create one OpenGL texture
    openGL_id = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
                                      SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

    clear_gl_error();

    if(openGL_id == 0) {
        std::cerr << "- SOIL loading error: " << SOIL_last_result() << '\n';
        return false;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, openGL_id);        //Bind texture to set options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);       //Unbind texture

    std::clog << "- Finished loading texture in " << glfwGetTime() - start_time << " seconds\n\n";
    return true;
}

void texture::bind_texture() {
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, openGL_id);
}

unsigned int texture::get_instance_count() {
    return instances;
}

void texture::add_instance() {
    instances++;
}

void texture::remove_instance() {
    if(instances > 0) {
        instances--;
    } else {
        std::cerr << "! Removed instance of non-used texture\n";
    }
}
