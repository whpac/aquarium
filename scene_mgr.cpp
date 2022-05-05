#include "scene_mgr.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "constants.h"
#include "allmodels.h"

namespace Scene {
	SceneManager::SceneManager(int wnd_width, int wnd_height){
		sp = nullptr;
		wnd_ratio = (float)wnd_width / wnd_height;
		window = glfwCreateWindow(wnd_width, wnd_height, "Aquarium", NULL, NULL);

		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);
	}

	void SceneManager::init() {
		initShaders();
		glEnable(GL_DEPTH_TEST);
	}

	void SceneManager::destroy() {
		for (auto& object : objects) {
			delete object;
		}
		objects.clear();
		freeShaders();
		glfwDestroyWindow(this->window);
	}

	void SceneManager::setShaderProgram(ShaderProgram* sp) {
		this->sp = sp;
	}

	void SceneManager::addObject(Object* obj) {
		objects.push_back(obj);
	}

	void SceneManager::draw() {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 V = glm::lookAt(
			glm::vec3(0.0f, 0.0f, -5.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, wnd_ratio, 1.0f, 50.0f);

		sp->use();//Activate shader program
		glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
		glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

		for (auto& object : objects) {
			object->draw(sp);
		}

		glfwSwapBuffers(window);
	}
}

