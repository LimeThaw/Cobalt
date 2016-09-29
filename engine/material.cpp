#include "material.h"

material::material(std::string arg_name, bool shadow_caster) : shadow_caster(shadow_caster) {
	mat_is_standard = false;
	if(arg_name == "") {
		name = name_manager::get_instance()->insert(this);
	} else {
		name = name_manager::get_instance()->insert(arg_name, this);
	}
}

material::material(const texture_bindings &textures, std::string arg_name, bool shadow_caster) : textures(textures) {
	material(arg_name, shadow_caster);
}

void material::set_uniform(std::string name, uniform::ptr new_uniform) {
	uniforms[name] = new_uniform;
}

void material::remove_uniform(std::string name) {
	if(uniforms.erase(name) == 0) {
		std::cerr << " ! Tried to remove non-existent uniform " << name << ".\n";
	}
}

json material::get_uniforms_json() {
	json uniforms;
	for(auto u : this->uniforms) {
		if(auto uni = dynamic_cast<float_uniform*>(u.second.get())) {
			uniforms[u.first] = *uni->get_data();
		}
	}
	return uniforms;
}

void material::add_texture(std::string uniform_name, std::shared_ptr<texture> tex) {
	for(unsigned int i = 0; i < textures.size(); ++i) {
		if(textures[i].first == uniform_name) {
			textures[i].second = tex;
			return;
		}
	}
    textures.push_back(std::pair<std::string, std::shared_ptr<texture>>(uniform_name, tex));
}

const material::texture_bindings &material::get_textures() {
	return textures;
}

void material::use() {
    GLint shader_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &shader_id);
    for(unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i + 1);
        textures[i].second->bind();
        glUniform1i(glGetUniformLocation((GLuint) shader_id, textures[i].first.c_str()), i + 1);
    }
    for(auto iterator : uniforms) {
    	iterator.second->bind(iterator.first);
    }
}

bool material::is_standard() {
	return mat_is_standard;
}

bool material::is_shadow_caster() {
	return shadow_caster;
}

void material::set_shadow_cast(bool shadow) {
	shadow_caster = shadow;
}

string material::set_name(const string arg_name) {
	name_manager::get_instance()->remove(name);
	name = name_manager::get_instance()->insert(arg_name, this);
	return name;
}

string material::get_name() {
	return name;
}
