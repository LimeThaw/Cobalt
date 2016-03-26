#ifndef STD_MATERIAL_H_INCLUDED
#define STD_MATERIAL_H_INCLUDED

#include "../cobalt.h"

namespace cs {

class std_material : public material {
	public:
		std_material(glm::vec3 color = glm::vec3(1.0f), float ref = 1.0f, float rough = 0.5f, float lumi = 0.0f, bool shadow_caster = true);
		void set_color(glm::vec3 new_color);
		void set_roughness(float new_roughness);
		void set_reflectivity(float new_reflectivity);
		void set_luminosity(float new_luminosity);
		void set_color_map(std::shared_ptr<texture2d> new_color_map);
		void set_color_map(std::string path);
		void set_normal_map(std::shared_ptr<texture2d> new_normal_map);
		void set_normal_map(std::string path);
		void set_bump_map(std::string path);
		void set_shader_mask(std::shared_ptr<texture2d> new_shader_mask);
		void set_shader_mask(std::string path);

	private:
		std::shared_ptr<texture2d> color_map;
		std::shared_ptr<texture2d> normal_map;
		std::shared_ptr<texture2d> shader_mask;
};

}

#endif
