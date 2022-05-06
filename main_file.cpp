#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "allmodels.h"
#include "constants.h"
#include "fish.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "scene_mgr.h"

#define WND_WIDTH 1000
#define WND_HEIGHT 800

//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {

	}

	if (action == GLFW_RELEASE) {

	}
}

int main(void)
{
	glfwSetErrorCallback(error_callback);//Register error processing callback procedure

	if (!glfwInit()) { //Initialize GLFW library
		fprintf(stderr, "Can't initialize GLFW.\n");
		exit(EXIT_FAILURE); 
	}

	// Initizalize the window
	Scene::SceneManager scene_mgr(WND_WIDTH, WND_HEIGHT);
	if (!scene_mgr.window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(scene_mgr.window, onKeyEvent);

	// Initialize the GLEW library
	GLenum err;
	if ((err=glewInit()) != GLEW_OK) { //Initialize GLEW library
		fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	// Initialize the scene and set shader program
	scene_mgr.init();
	scene_mgr.setShaderProgram(spLambert);

	/*auto obj = new Objects::Object();
	obj->setPosition(1, 0, 0);
	scene_mgr.addObject(obj);

	obj = new Objects::Object();
	obj->setPosition(0, 1, 0);
	scene_mgr.addObject(obj);*/

	auto fish = new Objects::Fish();
	fish->setPosition(0, 0, 0);
	fish->setRotation(0, PI / 6);
	scene_mgr.addObject(fish);

	glfwSetTime(0);

	//Main application loop
	while (!glfwWindowShouldClose(scene_mgr.window)) //As long as the window shouldnt be closed yet...
	{
		double currentTime = glfwGetTime();
		glfwSetTime(0);
		scene_mgr.draw();
		glfwPollEvents();
	}

	scene_mgr.destroy();
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}
