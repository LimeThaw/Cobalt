#include "std_scene.h"
#include <iostream>
using namespace cs;

std_scene::std_scene() : scene() {
	render_pass = std::make_shared<std_render_pass>();
}

void std_scene::set_camera(std::shared_ptr<camera> new_camera) {
	main_camera = new_camera;
}

void std_scene::add_point_light(point_light new_light) {
	p_lights.push_back(new_light);
}

void std_scene::add_directional_light(directional_light new_light) {
	d_lights.push_back(new_light);
}

void std_scene::render() {
	if(main_camera) {
		render_pass -> render(*this, *main_camera, d_lights, p_lights);
	} else {
		std::cerr << "! Tried to render scene without active camera\n";
	}
}
