#ifndef STD_RENDER_PASS_H_INCLUDED
#define STD_RENDER_PASS_H_INCLUDED

#include "../cobalt.h"
#include "../framebuffer.h"
#include <vector>
#include <memory>

namespace cs {

class std_render_pass : public render_pass<scene, camera, std::vector<directional_light>, std::vector<point_light>> {
	public:
		std_render_pass();
		void render(scene &a_scene, camera the_camera, std::vector<directional_light> d_lights, std::vector<point_light> p_lights) override;
		
	private:
		std::shared_ptr<shader> std_shader, depth_shader;
		unsigned int num_d_lights;
		unsigned int num_p_lights;
		std::vector<std::shared_ptr<texture2d>> shadow_maps;
		std::vector<std::shared_ptr<framebuffer>> shadow_map_framebuffers;
		material::texture_bindings shadow_map_bindings;
		std::vector<glm::mat4> shadow_map_view_projections;
};

}

#endif
