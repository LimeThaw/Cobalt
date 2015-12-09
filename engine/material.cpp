#include "material.h"


material::material() {
}

material::material(const texture_bindings &textures) : textures(textures) {

}

void material::set_uniform(std::string name, uniform *new_uniform) {
	uniforms[name] = new_uniform;
}

void material::remove_uniform(std::string name) {
	if(uniforms.erase(name) == 0) {
		std::cerr << " ! Tried to remove non-existent uniform " << name << ".\n";
	}
}

void material::add_texture(std::string uniform_name, std::shared_ptr<texture> tex) {
    textures.push_back(std::pair<std::string, std::shared_ptr<texture>>(uniform_name, tex));
}

void material::use() {
    GLint shader_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &shader_id);
    for(unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        textures[i].second->bind();
        glUniform1i(glGetUniformLocation((GLuint) shader_id, textures[i].first.c_str()), i);
    }
    for(auto iterator : uniforms) {
    	iterator.second->bind(iterator.first);
    }
}
