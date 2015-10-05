#include "material.h"


material::material() {
}

material::material(const texture_bindings &textures) : textures(textures) {

}

void material::add_texture(std::string uniform_name, std::shared_ptr<texture> tex) {
    textures.push_back(std::pair<std::string, std::shared_ptr<texture>>(uniform_name, tex));
}

void material::use() {
    for(unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        textures[i].second->bind();
    }
}
