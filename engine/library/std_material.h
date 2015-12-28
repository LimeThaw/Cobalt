#ifndef STD_MATERIAL_H_INCLUDED
#define STD_MATERIAL_H_INCLUDED

#include "../material.h"
#include "../uniforms.h"
#include "../texture2d.h"

namespace cs {

class std_material : public material {
	public:
		std_material(glm::vec3 color = glm::vec3(1.0f), float rough = 0.5f, float ref = 1.0f);
		void set_color_map(std::shared_ptr<texture2d> new_color_map);
		void set_normal_map(std::shared_ptr<texture2d> new_normal_map);
		void set_shader_mask(std::shared_ptr<texture2d> new_shader_mask);

	private:
		std::shared_ptr<texture2d> color_map;
		std::shared_ptr<texture2d> normal_map;
		std::shared_ptr<texture2d> shader_mask;
};

}

#endif
