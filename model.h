#ifndef MODEL_H
#define MODEL_H

#include<vector>
#include "model_loader.h"
#include "shaderprogram.h"

namespace Objects {
	class Model {
	protected:
		std::vector<float> vertices;
		std::vector<float> texture_coords;
		std::vector<float> normals;

	public:
		bool successful = false;

		Model();

		float* getVerticesPointer();
		float* getTextureCoordsPointer();
		float* getNormalsPointer();
		int getVertexCount();

		void draw(ShaderProgram* sp, GLuint texture, glm::mat4 M);
	};
}

#endif

