//
// Created by flocke on 05.10.15.
//

#ifndef COBALT_TEXTURE_DATA_SOURCE_H
#define COBALT_TEXTURE_DATA_SOURCE_H

#include <memory>
#include <GL/gl.h>

struct texture_data_source {
    GLsizei width, height;
    GLenum format;
    GLenum type;
    uint num_channels;
    std::shared_ptr<void> data;

    texture_data_source(GLsizei width, GLsizei height, GLenum format, GLenum type, std::shared_ptr<void> data);

    static GLenum num_channels_to_format(uint num_channels);

    static uint format_to_num_channels(GLenum format);

    static texture_data_source load_from_file(std::string filename);

    static texture_data_source create_normals_from_height(const texture_data_source &height_map);

    static texture_data_source create_null_data_source(GLsizei width, GLsizei height);
    
    bool operator==(const texture_data_source &other);
};


#endif //COBALT_TEXTURE_DATA_SOURCE_H
