#include "object.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include "allmodels.h"

namespace Objects {

	Object::Object(GLuint texture, Model& model) {
		this->texture = texture;
		this->model = model;
	}

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

	void Object::move(float dist) {
		this->x += dist * cos(this->rotUp) * cos(this->rotSide);
		this->y += dist * sin(this->rotUp);
		this->z -= dist * cos(this->rotUp) * sin(this->rotSide);
	}

	void Object::performMove(float time, float deltaTime) {
		// Objects don't move on their own
	}
}
