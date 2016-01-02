#include "../scene.h"
#include "std_render_pass.h"
#include <memory>

namespace cs {

class std_scene : public scene {
	public:
		std_scene();
		void set_camera(std::shared_ptr<camera> new_camera);
		void add_point_light(point_light new_light);
		void add_directional_light(directional_light new_light);
		void set_skybox(std::string path);
		void render();
	private:
		std::shared_ptr<std_render_pass> render_pass;
		std::shared_ptr<camera> main_camera;
		std::vector<directional_light> d_lights;
		std::vector<point_light> p_lights;
		std::shared_ptr<mesh> skybox;
		std::shared_ptr<shader> skybox_shader;
};

}
