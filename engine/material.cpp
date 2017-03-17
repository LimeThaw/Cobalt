#include "material.h"

/**
 *  The basic constructor of the class, providing only the information necessary for the object
 *  to make sense. All parameters are optional and have default values.
 *  @param	arg_name		The name of the newly created object. It can later be found by this
 *  							name in the name_manager. If no name is provided the name_manager
 *  							will generate and assign it a unique name of the form Item_X.
 *  @param	shadow_caster	Decides wether the object should cast shadows in the scene. The default
 *  							value is true, so you only need to provide this argument for some
 *  							transparent or half-invisible objects or something.
 */
material::material(std::string arg_name, bool shadow_caster) : named(arg_name), shadow_caster(shadow_caster) {
	mat_is_standard = false;
}

/**
 *  Slightly extended constructor that takes texture_bindings and assigns them to the material, in
 *  addition to the functionality provided by the basic constructor.
 *  @param	textures	texture_bindings containing the textures to be assigned to the material, and
 *  						their bindings.
 */
material::material(const texture_bindings &textures, std::string arg_name, bool shadow_caster) : textures(textures) {
	material(arg_name, shadow_caster);
}

/**
 *  This method sets a specific shader uniform for the material, aka a specific value given to
 *  the shader at a particular location. The implementation can be found in uniforms.cpp.
 *  @param	name		The name assigned to the uniform. It can later be used to remove or update it.
 *  @param	new_uniform	The uniform to be stored, or more specifically a shared_ptr to it (aka uniform::ptr).
 *  						Note that uniform itself is an abstract class, and you need to use the right
 *  						subclass for your data type. See uniforms.h for reference.
 */
void material::set_uniform(std::string name, uniform::ptr new_uniform) {
	uniforms[name] = new_uniform;
}

/**
 *  Method to remove a uniform from the material. If you needed a value in the shader, but now you
 *  don't need it anymore, this is a handy method to use.
 *  @param	name	The name of the uniform. It is the same one that was given in set_uniform().
 */
void material::remove_uniform(std::string name) {
	if(uniforms.erase(name) == 0) {
		std::cerr << " ! Tried to remove non-existent uniform " << name << ".\n";
	}
}

/**
 *  This converts all uniform names and values to a nice json output and gives back the string.
 *  The method was primarily intended for use in cs::std_scene::save().
 */
json material::get_uniforms_json() {
	json uniforms;
	for(auto u : this->uniforms) {
		if(auto uni = dynamic_cast<float_uniform*>(u.second.get())) {
			uniforms[u.first].push_back("float");
			uniforms[u.first].push_back(*uni->get_data());
		} else if(auto uni = dynamic_cast<vec3_uniform*>(u.second.get())) {
			uniforms[u.first].push_back("vec3");
			uniforms[u.first].push_back(serialize(*uni->get_data()));
		}
	}
	return uniforms;
}

/**
 *  Add a texture to the material. If bound properly can be accessed in the shader as a sampler under
 *  its assigned name.
 *  @param	uniform_name	The name of the texture, also used in the shader.
 *  @param	tex				The actual texture to be used, or actually a shared_ptr. You can use a
 *  							texture_ptr too. See texture for more details on textures.
 */
void material::add_texture(std::string uniform_name, std::shared_ptr<texture> tex) {
	for(unsigned int i = 0; i < textures.size(); ++i) {
		if(textures[i].first == uniform_name) {
			textures[i].second = tex;
			return;
		}
	}
    textures.push_back(std::pair<std::string, std::shared_ptr<texture>>(uniform_name, tex));
}

/**
*  A simple get-method returning all textures assigned to the material and their bindings.
*/
const material::texture_bindings &material::get_textures() {
	return textures;
}

/**
 *  "Activation mehtod" of the class. It sets all uniforms and textures assigned to the material.
 *  If you want to render something with the material, you need to call this first.
 */
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

/**
*  Resets all bound textures. Might be handy to avoid weird errors and anomalies.
*/
void material::unbind() {
    GLint shader_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &shader_id);
    for(unsigned int i = 0; i < textures.size(); i++) {
        glUniform1i(glGetUniformLocation((GLuint) shader_id, textures[i].first.c_str()), 0);
    }
}
/**
*  Find out wether or not the material is standard.
*  If it is, it is of class cs::std_material.
*/
bool material::is_standard() {
	return mat_is_standard;
}

/**
*  Find out wether an object with this material should cast shadows.
*/
bool material::is_shadow_caster() {
	return shadow_caster;
}

/**
*  Set the shadow-casting properties of the material.
*  If set to false, objects with this material will be ignored during shadow map generation.
*/
void material::set_shadow_cast(bool shadow) {
	shadow_caster = shadow;
}