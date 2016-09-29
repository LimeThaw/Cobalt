#ifndef STD_SHADER_H_INCLUDED
#define STD_SHADER_H_INCLUDED

#include "../shader.h"

namespace cs {

class std_shader {
	public:
		std_shader();
		void use();
	private:
		shader actual_shader;
};

}

#endif
