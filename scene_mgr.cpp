#include "scene_mgr.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "global.h"


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
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		glClearColor(0.0f, 0.0f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*glm::mat4 V = glm::lookAt(
			glm::vec3(0.0f, 0.0f, -25.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, wnd_ratio, 1.0f, 50.0f);*/


		glm::mat4 V = glm::lookAt(pos, pos + dir, glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku
		glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 0.1f, 50.0f); //Wylicz macierz rzutowania


		sp->use();//Activate shader program
		glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
		glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

		for (auto& object : objects) {
			object->draw(sp);
		}

		glfwSwapBuffers(window);
	}

	void SceneManager::performMoves(float time, float deltaTime) {
		for (auto& object : objects) {
			object->performMove(time, deltaTime);
		}
	}

	GLuint SceneManager::readTexture(const char* filename) {
		GLuint tex;
		glActiveTexture(GL_TEXTURE0);
		//Read into computers memory
		std::vector<unsigned char> image; //Allocate memory
		unsigned width, height; //Variables for image size
		//Read the image
		unsigned error = lodepng::decode(image, width, height, filename);
		//Import to graphics card memory
		glGenTextures(1, &tex); //Initialize one handle
		glBindTexture(GL_TEXTURE_2D, tex); //Activate handle
		//Copy image to graphics cards memory reprezented by the active handle
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		return tex;
	}
}

