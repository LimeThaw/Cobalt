#include "std_material.h"
using namespace cs;

std_material::std_material(glm::vec3 color, float rough, float ref) {
	set_uniform("material_color", std::make_shared<vec3_uniform>(color));
	set_uniform("material_roughness", std::make_shared<float_uniform>(rough));
	set_uniform("material_reflectivity", std::make_shared<float_uniform>(ref));
	mat_is_standard = true;
	set_uniform("has_color_map", std::make_shared<bool_uniform>(false));
}

void std_material::set_color_map(std::shared_ptr<texture2d> new_color_map) {
	color_map = new_color_map;
	add_texture("color_map", new_color_map);
	set_uniform("has_color_map", std::make_shared<bool_uniform>(true));
}
