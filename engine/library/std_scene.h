#include "../scene.h"
#include "../material.h"
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
		void set_camera(std::shared_ptr<camera> new_camera);
		void add_point_light(std::shared_ptr<point_light> new_light);
		void add_directional_light(std::shared_ptr<directional_light> new_light);
		void set_skybox(std::string path);
        void save(const char* file_path, bool pretty = true);
		void render();
	private:
		std::shared_ptr<std_render_pass> render_pass;
		std::shared_ptr<camera> main_camera;
		std::vector<std::shared_ptr<directional_light>> d_lights;
		std::vector<std::shared_ptr<point_light>> p_lights;
		std::shared_ptr<mesh> skybox;
		std::string skybox_texture_path;
		std::shared_ptr<shader> skybox_shader;
};

}
