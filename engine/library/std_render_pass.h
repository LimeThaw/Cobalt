#ifndef STD_RENDER_PASS_H_INCLUDED
#define STD_RENDER_PASS_H_INCLUDED

#include "../cobalt.h"
#include <vector>
#include <memory>

namespace cs {

class std_render_pass : public render_pass<scene, camera, std::vector<directional_light>, std::vector<point_light>> {
	public:
		std_render_pass();
		void render(scene &a_scene, camera the_camera, std::vector<directional_light> d_lights, std::vector<point_light> p_lights) override;
};

}

#endif
