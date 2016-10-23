#include "texture.h"

/**
 *  Constructor of the class. Generates a new OpenGL texture name and holds it, sets its own
 *  path to the empty string and its name to the given argument.
 *  @param	arg_name	The name of the new texture.
 */
texture::texture(std::string arg_name) {
    glGenTextures(1, &openGL_id);
	name = name_manager::get_instance()->insert(arg_name, this);
	path = "";
}

/** Destructor of the class. Frees the OpenGL buffer and removes its name from the name_manager. */
texture::~texture() {
    glDeleteTextures(1, &openGL_id);
    name_manager::get_instance()->remove(name);
}

/** Returns the value of the id that identifies the texture to OpenGL. */
GLuint texture::get_openGL_id() {
    return openGL_id;
}

/**
 *  Sets the name of the texture to the given argument. Removes the old name from the name_manager
 *  and registers the new one.
 */
std::string texture::set_name(const std::string arg_name) {
	name_manager::get_instance()->remove(name);
	name = name_manager::get_instance()->insert(arg_name, this);
	return name;
}

/** Returns the value of the name field */
std::string texture::get_name() {
	return name;
}

/**
 *  Returns the value of the path field. This is the path where the texture was loaded from if it was
 *  loaded from disk.
 */
std::string texture::get_path() {
	return path;
}

/**
 *  \fn virtual void texture::bind()
 *  Makes the texture current in the appropriate OpenGL context.
 *  This purely virtual function is implemented by the descendants of the texture class, because
 *  different types of textures (cubemaps, 2D textures, etc.) belong into different OpenGL contexts.
 */