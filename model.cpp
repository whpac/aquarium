#include "model.h"
#include <glm/gtc/type_ptr.hpp>

namespace Objects {

	Model::Model() {}

	float* Model::getVerticesPointer() {
		return &vertices[0];
	}

	float* Model::getTextureCoordsPointer() {
		return &texture_coords[0];
	}

	float* Model::getNormalsPointer() {
		return &normals[0];
	}

	int Model::getVertexCount() {
		return vertices.size() / 4;
	}

	void Model::draw(ShaderProgram* sp, GLuint texture, glm::mat4 position_matrix) {
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(position_matrix));
		glUniform4f(sp->u("tintColor"), 0, 0, 0.8, 1);

		glEnableVertexAttribArray(sp->a("vertex"));
		glEnableVertexAttribArray(sp->a("normal"));
		glEnableVertexAttribArray(sp->a("texCoord"));

		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, getVerticesPointer());
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, getNormalsPointer());
		glVertexAttribPointer(sp->a("texCoord"), 2, GL_FLOAT, false, 0, getTextureCoordsPointer());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(sp->u("texture"), 0);
		glDrawArrays(GL_TRIANGLES, 0, getVertexCount());

		glDisableVertexAttribArray(sp->a("vertex"));
		glDisableVertexAttribArray(sp->a("normal"));
		glDisableVertexAttribArray(sp->a("texCoord"));
	}
}
