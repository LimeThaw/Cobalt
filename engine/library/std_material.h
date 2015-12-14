#ifndef STD_MATERIAL_H_INCLUDED
#define STD_MATERIAL_H_INCLUDED

#include "../material.h"
#include "../uniforms.h"

namespace cs {

class std_material : public material {
	public:
		std_material(glm::vec3 color = glm::vec3(1.0f), float rough = 0.5f, float ref = 1.0f);
};

}

#endif
