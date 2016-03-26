#include "std_material.h"
using namespace cs;

std_material::std_material(glm::vec3 color, float ref, float rough, float lumi, bool shadow_caster) : material(shadow_caster) {
	set_uniform("material_color", std::make_shared<vec3_uniform>(color));
	set_uniform("material_reflectivity", std::make_shared<float_uniform>(ref));
	set_uniform("material_roughness", std::make_shared<float_uniform>(rough));
	set_uniform("material_luminosity", std::make_shared<float_uniform>(lumi));
	mat_is_standard = true;
	set_uniform("has_color_map", std::make_shared<bool_uniform>(false));
	set_uniform("has_normal_map", std::make_shared<bool_uniform>(false));
	set_uniform("has_shader_mask", std::make_shared<bool_uniform>(false));
}

void std_material::set_color(glm::vec3 new_color) {
	set_uniform("material_color", std::make_shared<vec3_uniform>(new_color));
}

void std_material::set_roughness(float new_roughness) {
	set_uniform("material_roughness", std::make_shared<float_uniform>(new_roughness));
}

void std_material::set_reflectivity(float new_reflectivity) {
	set_uniform("material_reflectivity", std::make_shared<float_uniform>(new_reflectivity));
}

void std_material::set_luminosity(float new_luminosity) {
	set_uniform("material_luminosity", std::make_shared<float_uniform>(new_luminosity));
}

void std_material::set_color_map(std::shared_ptr<texture2d> new_color_map) {
	color_map = new_color_map;
	add_texture("color_map", new_color_map);
	set_uniform("has_color_map", std::make_shared<bool_uniform>(true));
}

void std_material::set_color_map(std::string path) {
	auto texture = std::make_shared<texture2d>(
            texture_cache::get_instance().get_texture_from_filename(path));
	set_color_map(texture);
}

void std_material::set_normal_map(std::shared_ptr<texture2d> new_normal_map) {
	normal_map = new_normal_map;
	add_texture("normal_map", new_normal_map);
	set_uniform("has_normal_map", std::make_shared<bool_uniform>(true));
}

void std_material::set_normal_map(std::string path) {
	auto texture = std::make_shared<texture2d>(
            texture_cache::get_instance().get_texture_from_filename(path));
	set_normal_map(texture);
}

void std_material::set_bump_map(std::string path) {
	auto texture = std::make_shared<texture2d>(
			texture_data_source::create_normals_from_height(
            texture_cache::get_instance().get_texture_from_filename(path)));
	set_normal_map(texture);
}

void std_material::set_shader_mask(std::shared_ptr<texture2d> new_shader_mask) {
	shader_mask = new_shader_mask;
	add_texture("shader_mask", new_shader_mask);
	set_uniform("has_shader_mask", std::make_shared<bool_uniform>(true));
}

void std_material::set_shader_mask(std::string path) {
	auto texture = std::make_shared<texture2d>(
            texture_cache::get_instance().get_texture_from_filename(path));
	set_shader_mask(texture);
}
