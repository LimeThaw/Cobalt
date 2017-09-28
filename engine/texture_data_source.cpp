//
// Created by flocke on 05.10.15.
//

#include <iostream>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include "texture_data_source.h"
#include "util.h"

/**
 *  The class constructor. Takes the raw image data and important metadata and stores it in the
 *  object.
 */
texture_data_source::texture_data_source(GLsizei width, GLsizei height, GLenum format, GLenum type,
                                         std::shared_ptr<void> data, bool invert) :
        width(width), height(height), format(format), type(type), data(data) {
    num_channels = format_to_num_channels(format);
	if(invert) invert_y();
}

/**
 *  Converts the number of desired chanels into an OpenGL color format.
 */
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

/**
 *  Returns the number of channels in the given OpenGL color format.
 */
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

/**
 *  Loads an image from a file and returns the resulting texture_data_source. If loading fails
 *  this function will throw an std::runtime_error.
 *  @param	filename	The path of the image file you want to load
 *  @param	invert_y	If true the pixels of the image will be flipped in the y direction. See
 *  						texture_data_source::invert_y().
 */
texture_data_source texture_data_source::load_from_file(std::string filename, bool invert_y) {
    GLsizei width, height;
    int channels;
    unsigned char *data = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
    if(data == nullptr) {
        throw std::runtime_error("error loading texture " + filename + ": " + SOIL_last_result());
    } else {
    	std::clog << indent::get() << "- Loaded texture " << filename << "\n";
    }
    return texture_data_source(width, height, num_channels_to_format((uint) channels), GL_UNSIGNED_BYTE,
                               std::shared_ptr<void>(data), invert_y);
}

/**
 *  Takes a greyscale heightmap and computes the corresponding normal map. Both input and
 *  output are given as texture_data_source objects.
 *  @param	height_map	The heightmap that you want to turn into a normal map.
 */
texture_data_source texture_data_source::create_normals_from_height(const texture_data_source &height_map) {
    if(height_map.type != GL_BYTE && height_map.type != GL_UNSIGNED_BYTE) {
        throw std::runtime_error("unsupported input for create_normals_from_height");
    }

    unsigned char *height_data = (unsigned char *) height_map.data.get();
    GLsizei width = height_map.width, height = height_map.height;

    unsigned char *rst_data = new unsigned char[width * height * 3];

    auto read_height = [&] (int x, int y) -> float {
        return height_data[(y * width + x) * height_map.num_channels] / 255.0f;
    };

    for(int iy = 0; iy < height; ++iy) {
        for(int ix = 0; ix < width; ++ix) {
            auto get_normal_by_offsets = [&] (int dx, int dy) -> glm::vec3 {
                glm::vec3 a(dx, 0, read_height(ix + dx, iy));
                glm::vec3 b(0, dy, read_height(ix, iy + dy));
                glm::vec3 c(0, 0, read_height(ix, iy));
                glm::vec3 rst = glm::cross(a - c, b - c);

                if(rst != glm::vec3(0)) {
                    //Turn around normals that go into the plane
                    return glm::normalize(rst * rst.z);
                } else {
                    return glm::vec3(0);
                }
            };
            glm::vec3 normal;
            if (ix > 0 && iy > 0) {
                normal += get_normal_by_offsets(-1, -1);
            }
            if (ix < width - 1 && iy > 0) {
                normal += get_normal_by_offsets(1, -1);
            }
            if (ix < width - 1 && iy < height - 1) {
                normal += get_normal_by_offsets(1, 1);
            }
            if (ix > 0 && iy < height - 1) {
                normal += get_normal_by_offsets(-1, 1);
            }
            normal = glm::normalize(normal);
            normal += glm::vec3(1.0);
            normal *= glm::vec3(0.5);
            rst_data[(iy * height + ix) * 3] = normalized_float_to_byte(normal.x);
            rst_data[(iy * height + ix) * 3 + 1] = normalized_float_to_byte(normal.y);
            rst_data[(iy * height + ix) * 3 + 2] = normalized_float_to_byte(normal.z);
        }
    }

    return texture_data_source(width, height, GL_RGB, GL_UNSIGNED_BYTE, std::shared_ptr<void>(rst_data));
}

/**
 *  Creates a data source without any data. This means the pointer to the image data will be null.
 *  The format is GL_RED / GL_UNSIGNED_BYTE.
 */
texture_data_source texture_data_source::create_null_data_source(GLsizei width, GLsizei height) {
    return texture_data_source(width, height, GL_RED, GL_UNSIGNED_BYTE, std::shared_ptr<void>());
}

/**
 *  Comparison operater. Returns true iff the pointers to the data sets of this and the given
 *  object are equal.
 */
bool texture_data_source::operator== (const texture_data_source &other) {
	return data == other.data;
}

/**
 *  Flips the texture on its horizontal axis, inverting it on the y axis.
 */
void texture_data_source::invert_y() {
	int i, j;
	auto d = (unsigned char*) data.get();
	for( j = 0; j*2 < height; ++j ) {
		int index1 = j * width * num_channels;
		int index2 = (height - 1 - j) * width * num_channels;
		for( i = width * num_channels; i > 0; --i ) {
			unsigned char temp = d[index1];
			d[index1] = d[index2];
			d[index2] = temp;
			++index1;
			++index2;
		}
	}
}