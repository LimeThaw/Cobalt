#include "texture.h"
#include "gl_exception.h"
#include "util.h"
#include <glm/glm.hpp>

texture::texture() {
    openGL_id = 0;
}

texture::~texture() {
    glDeleteTextures(1, &openGL_id);
}

bool texture::load(const std::string &path) {
    std::clog << "-Loading texture " << path << '\n';
    float start_time = glfwGetTime();

    // Create one OpenGL texture
    openGL_id = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
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

void texture::load_normalmap_from_heightmap(const std::string &path) {
    std::clog << "-Loading texture " << path << '\n';
    float start_time = glfwGetTime();
    
    
    int width, height, channels;
    unsigned char *height_data = SOIL_load_image(path.c_str(), &width, &height, &channels, SOIL_LOAD_L);
    
    if(height_data == nullptr) {
        throw std::runtime_error(SOIL_last_result());
    }
    
    std::clog << "- Finished loading texture in " << glfwGetTime() - start_time << " seconds\n\n";
    
    std::clog << "-Converting to normal map " << path << '\n';
    start_time = glfwGetTime();
    
    auto read_height = [&] (int x, int y) -> float { 
        return height_data[y * width + x] / 255.0f;
    };
    
    unsigned char *rst_data = new unsigned char[width * height * 3];
    for(int iy = 0; iy < height; ++iy) {
        for(int ix = 0; ix < width; ++ix) {
            auto get_normal_by_offsets = [&] (int dx, int dy) -> glm::vec3 { 
                glm::vec3 a(dx, 0, read_height(ix + dx, iy));
                glm::vec3 b(0, dy, read_height(ix, iy + dy));
                glm::vec3 c(0, 0, read_height(ix, iy));
                glm::vec3 rst = glm::cross(a - c, b - c);
                //Turn around normals that go into the plane
                return rst * rst.z;
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
    
    for(int i = 0; i < width * height; ++i) {
        //std::cerr << rst_data[i * 3] << ", " << rst_data[i * 3 + 1] << ", " << rst_data[i * 3 + 2] << std::endl;
    }
        
    std::clog << "- Finished converting to normal map in " << glfwGetTime() - start_time << " seconds\n\n";
    
    glGenTextures(1, &openGL_id);
    glBindTexture(GL_TEXTURE_2D, openGL_id);
    glTexStorage2D(GL_TEXTURE_2D, get_num_mipmap_levels(width, height), GL_RGB8, width, height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, rst_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    delete rst_data;
    SOIL_free_image_data(height_data);
}


void texture::bind_texture() {
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

int texture::get_num_mipmap_levels(int width, int height) {
    int d = std::min(width, height);
    return (int) log2(d) + 1;
}

