#ifndef STD_RENDER_PASS_H_INCLUDED
#define STD_RENDER_PASS_H_INCLUDED

#include "../cobalt.h"
#include "../framebuffer.h"
#include "../mesh.h"
#include "../pointer_wrapper.h"
#include <vector>
#include <memory>
#include <glm/gtx/transform.hpp>

namespace cs {

class std_render_pass : public render_pass<scene, camera_ptr, std::vector<directional_light_ptr>, std::vector<point_light_ptr>> {
	public:
		std_render_pass();
		~std_render_pass();
		void render(scene &a_scene, camera_ptr the_camera, std::vector<directional_light_ptr> d_lights, std::vector<point_light_ptr> p_lights) override;

	private:
		shader_ptr std_shader, depth_shader;
		unsigned int num_d_lights;
		unsigned int num_p_lights;
		std::vector<std::shared_ptr<texture2d>> directional_shadow_maps;
		std::vector<std::shared_ptr<framebuffer>> directional_shadow_map_framebuffers;
		std::vector<glm::mat4> directional_shadow_map_view_projections;
		std::vector<std::shared_ptr<cubemap>> point_shadow_maps;
		std::vector<std::shared_ptr<framebuffer>> point_shadow_map_framebuffers;
		glm::mat4 get_light_matrix(glm::vec3 direction, scene &a_scene);
};

typedef pointer_wrapper<std_render_pass> std_render_pass_ptr;

}

#endif
