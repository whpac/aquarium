#include "object.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "allmodels.h"

namespace Objects {

	glm::mat4 Object::makePositionMatrix() {
		glm::mat4 M(1.0f);
		M = glm::translate(M, glm::vec3(x, y, z));
		return M;
	}

	void Object::setPosition(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void Object::draw(ShaderProgram* sp) {
		auto M = makePositionMatrix();
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
		Models::sphere.drawSolid();
	}
}
