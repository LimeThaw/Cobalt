//
// Created by flocke on 05.10.15.
//

#include <SOIL/SOIL.h>
#include "texture_data_source.h"

texture_data_source::texture_data_source(GLsizei width, GLsizei height, GLenum format, GLenum type,
                                         std::shared_ptr<void> data) :
        width(width), height(height), format(format), type(type), data(data) {
    num_channels = format_to_num_channels(format);
}

GLenum texture_data_source::num_channels_to_format(uint num_channels) {
    switch(num_channels) {
        case 1:
            return GL_RED;
        case 2:
            return GL_RG;
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
        default:
            throw std::runtime_error("invalid number of channels for texture");
    }
}

uint texture_data_source::format_to_num_channels(GLenum format) {
    switch(format) {
        case GL_RED:
            return 1;
        case GL_RG:
            return 2;
        case GL_RGB:
        case GL_BGR:
            return 3;
        case GL_RGBA:
        case GL_BGRA:
            return 4;
        default:
            throw std::runtime_error("invalid format enum");
    }
}

texture_data_source texture_data_source::load_from_file(std::string filename) {
    GLsizei width, height;
    int channels;
    unsigned char *data = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
    if(data == nullptr) {
        throw std::runtime_error("error loading texture " + filename + ": " + SOIL_last_result());
    }
    return texture_data_source(width, height, num_channels_to_format((uint) channels), GL_UNSIGNED_BYTE,
                               std::shared_ptr<void>(data));
}

texture_data_source texture_data_source::create_null_data_source(GLsizei width, GLsizei height) {
    return texture_data_source(width, height, GL_RED, GL_UNSIGNED_BYTE, std::shared_ptr<void>());
}
