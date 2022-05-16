#include "fish.h"
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include "allmodels.h"
#include "model_loader.h"
#include<iostream>

namespace Objects {

	Fish::Fish(GLuint texture) {
		this->texture = texture;
		bool res = loadObj("clownfish.obj", vertices, texture_coords, normals);
		if (!res) {
			std::cout << "Failed to load model" << std::endl;
		}
		else {
			std::cout << "Read data successfully!" << std::endl;
			std::cout << vertices.size() / 4 << " vertices" << std::endl;
			std::cout << texture_coords.size() / 2 << " UVs" << std::endl;
			std::cout << normals.size() / 4 << " normals" << std::endl;
		}
	}

	void Fish::draw(ShaderProgram* sp) {
		auto M = makePositionMatrix();
		M = glm::rotate(M, -PI / 2, glm::vec3(0, 1, 0));
		M = glm::scale(M, glm::vec3(0.5, 0.5, 0.5));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
		//Models::cube.drawSolid();

		glEnableVertexAttribArray(sp->a("vertex"));
		glEnableVertexAttribArray(sp->a("normal"));
		glEnableVertexAttribArray(sp->a("texCoord"));

		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, &vertices[0]);
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, &normals[0]);
		glVertexAttribPointer(sp->a("texCoord"), 2, GL_FLOAT, false, 0, &texture_coords[0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(sp->u("tex"), 0);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 4);

		glDisableVertexAttribArray(sp->a("vertex"));
		glDisableVertexAttribArray(sp->a("normal"));
		glDisableVertexAttribArray(sp->a("texCoord"));
	}

	void Fish::performMove(float time, float deltaTime) {
		this->rotSide = time;
		return;
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
