#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "allmodels.h"
#include "constants.h"
#include "fish.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "scene_mgr.h"
#include "model.h"

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
	srand(time(nullptr));
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
	GLuint tex = scene_mgr.readTexture("clownfish.png");
	scene_mgr.setShaderProgram(spLambertTextured);

	auto clownfish_model = Objects::Model("clownfish.obj");
	auto fish = new Objects::Fish(tex, clownfish_model);
	scene_mgr.addObject(fish);

	glfwSetTime(0);
	double lastTime = 0;

	//Main application loop
	while (!glfwWindowShouldClose(scene_mgr.window)) //As long as the window shouldnt be closed yet...
	{
		scene_mgr.draw();
		double currentTime = glfwGetTime();
		scene_mgr.performMoves(currentTime, currentTime - lastTime);
		lastTime = currentTime;
		glfwPollEvents();
	}

	scene_mgr.destroy();
	glDeleteTextures(1, &tex);
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}
