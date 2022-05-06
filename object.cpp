#include "object.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "allmodels.h"

namespace Objects {

	glm::mat4 Object::makePositionMatrix() {
		glm::mat4 M(1.0f);
		M = glm::translate(M, glm::vec3(x, y, z));
		M = glm::rotate(M, rotSide, glm::vec3(0, 1, 0));
		M = glm::rotate(M, rotUp, glm::vec3(0, 0, 1));
		return M;
	}

	void Object::setPosition(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void Object::setRotation(float side, float up) {
		this->rotSide = side;
		this->rotUp = up;
	}

	void Object::draw(ShaderProgram* sp) {
		auto M = makePositionMatrix();
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
		Models::sphere.drawSolid();
	}
}
