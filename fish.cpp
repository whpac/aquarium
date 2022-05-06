#include "fish.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "allmodels.h"

namespace Objects {

	void Fish::draw(ShaderProgram* sp) {
		auto M = makePositionMatrix();
		M = glm::scale(M, glm::vec3(1, 1, 0.2));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
		Models::cube.drawSolid();
		this->move(0.01);
	}
}
