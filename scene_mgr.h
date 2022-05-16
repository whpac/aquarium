#ifndef SCENE_MGR_H
#define SCENE_MGR_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "object.h"
#include "shaderprogram.h"

using namespace std;
using namespace Objects;

namespace Scene {
	class SceneManager {
	protected:
		float wnd_ratio;
		ShaderProgram* sp;
		vector<Object*> objects;

	public:
		GLFWwindow* window;

		SceneManager(int wnd_width, int wnd_height);
		void init();
		void destroy();

		void setShaderProgram(ShaderProgram* sp);
		GLuint readTexture(const char* filename);

		void draw();
		void performMoves(float time, float deltaTime);

		void addObject(Object* obj);
	};
}

#endif
