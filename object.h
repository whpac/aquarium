#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include "shaderprogram.h"

namespace Objects {
	class Object {

	protected:
		glm::mat4 makePositionMatrix();

	public:
		float x = 0, y = 0, z = 0;
		float rotSide = 0, rotUp = 0;

		void setPosition(float x, float y, float z);
		void setRotation(float side, float up);
		void move(float dist);

		virtual void performMove(float deltaTime);
		virtual void draw(ShaderProgram* sp);
	};
}

#endif