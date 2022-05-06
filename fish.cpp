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
	}

	void Fish::performMove(float time, float deltaTime) {
		float velocity = 1;
		if (turnStart < 0) {
			int r = rand() % 256;
			if (r == 0) {
				turnStart = time;
				turnEnd = time + PI / 2;
				rotateTarget = 2 * (((double)rand() / RAND_MAX) - 0.5);
			}
		}

		float direction_mod = 0;
		if (turnStart > 0) {
			direction_mod = sin(time - turnStart) * rotateTarget;
		}

		if (time > turnEnd) {
			turnStart = -1;
			turnEnd = -1;
			rotateTarget = 0;
			direction += direction_mod;
			direction_mod = 0;
		}

		this->rotSide = direction + direction_mod + sin(time) * PI / 24;
		this->move(velocity * deltaTime);
	}
}
