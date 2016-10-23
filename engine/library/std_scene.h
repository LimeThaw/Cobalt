#ifndef STD_SCENE_H_INCLUDED
#define STD_SCENE_H_INCLUDED

#include "../scene.h"
#include "../material.h"
#include "std_material.h"
#include "std_render_pass.h"
#include <memory>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "../json.hpp"
using json = nlohmann::json;

namespace cs {

class std_scene : public scene {
	public:
		std_scene();
		~std_scene();
		void set_camera(camera* new_camera);
		void add_point_light(point_light* new_light);
		void add_directional_light(directional_light* new_light);
		void set_skybox(std::string path);
        void save(const char* file_path, bool pretty = true);
        void load(const char* file_path);
		void render();
	private:
		std_render_pass* render_pass;
		camera* main_camera;
		std::vector<directional_light*> d_lights;
		std::vector<point_light*> p_lights;
		mesh* skybox;
		std::string skybox_texture_path;
		shader* skybox_shader;
		std::vector<std::pair<char, void*>> to_delete;
};

}

#endif