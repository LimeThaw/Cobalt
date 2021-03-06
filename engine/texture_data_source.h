//
// Created by flocke on 05.10.15.
//

#ifndef COBALT_TEXTURE_DATA_SOURCE_H
#define COBALT_TEXTURE_DATA_SOURCE_H

#include <memory>
#include <GL/gl.h>

#include "indent.h"

/**
 *  A class representing texture data in memory. It stores the raw image data along with important
 *  metadata. This allows to buffer loaded texture data and share it among several texture objects.
 */
struct texture_data_source {
    GLsizei width, height;
	/**
	 *  See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml at the
	 *  format parameter for options and more info.
	 */
    GLenum format;
	/**
	 *  See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml at the
	 *  type parameter for options and more info.
	 */
    GLenum type;
    uint num_channels;
    std::shared_ptr<void> data;

    texture_data_source(GLsizei width, GLsizei height, GLenum format, GLenum type, std::shared_ptr<void> data, bool invert = true);

    static GLenum num_channels_to_format(uint num_channels);

    static uint format_to_num_channels(GLenum format);

    static texture_data_source load_from_file(std::string filename, bool invert_y = true);

    static texture_data_source create_normals_from_height(const texture_data_source &height_map);

    static texture_data_source create_null_data_source(GLsizei width, GLsizei height);

    bool operator==(const texture_data_source &other);

	void invert_y();
};


#endif //COBALT_TEXTURE_DATA_SOURCE_H
