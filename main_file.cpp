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
#include "global.h"
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#define WND_WIDTH 1000
#define WND_HEIGHT 800


float speed_y = 0; //[radiany/s]
float speed_x = 0; //[radiany/s]
float ws = 0;
using namespace glm;

vec3 pos = vec3(0, 1, -5);
vec3 dir = vec3(0, 0, 1);
vec3 Up = vec3(0.0f, 1.0f, 0.0f);

// Prevents the camera from jumping around when first clicking left click
bool firstClick = true;


//float speed = 0.1f;
float sensitivity = 100.0f;






void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speed_y = 1;
		if (key == GLFW_KEY_RIGHT) speed_y = -1;
		if (key == GLFW_KEY_PAGE_UP) speed_x = 1;
		if (key == GLFW_KEY_PAGE_DOWN) speed_x = -1;
		if (key == GLFW_KEY_UP) ws = 5;
		if (key == GLFW_KEY_DOWN) ws = -5;


		if (key == GLFW_MOUSE_BUTTON_LEFT)
		{
			// Hides mouse cursor
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			// Prevents camera from jumping on the first click
			if (firstClick)
			{
				glfwSetCursorPos(window, (WND_WIDTH / 2), (WND_HEIGHT / 2));
				firstClick = false;
			}

			// Stores the coordinates of the cursor
			double mouseX;
			double mouseY;
			// Fetches the coordinates of the cursor
			glfwGetCursorPos(window, &mouseX, &mouseY);

			// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
			// and then "transforms" them into degrees 
			float rotX = sensitivity * (float)(mouseY - (WND_HEIGHT / 2)) / WND_HEIGHT;
			float rotY = sensitivity * (float)(mouseX - (WND_WIDTH / 2)) / WND_HEIGHT;

			// Calculates upcoming vertical change in the Orientation
			glm::vec3 newOrientation = glm::rotate(dir, glm::radians(-rotX), glm::normalize(glm::cross(dir,Up)));

			// Decides whether or not the next vertical Orientation is legal or not
			if (abs(glm::angle(newOrientation,Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			{
				dir = newOrientation;
			}

			// Rotates the Orientation left and right
			dir = glm::rotate(dir, glm::radians(-rotY), Up);

			// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
			glfwSetCursorPos(window, (WND_WIDTH / 2), (WND_HEIGHT / 2));

		}






	}

	if (action == GLFW_RELEASE) {

		if (key == GLFW_KEY_LEFT) speed_y = 0;
		if (key == GLFW_KEY_RIGHT) speed_y = 0;

		if (key == GLFW_KEY_UP) ws = 0;
		if (key == GLFW_KEY_DOWN) ws = 0;

		if (key == GLFW_KEY_PAGE_UP) speed_x = 0;
		if (key == GLFW_KEY_PAGE_DOWN) speed_x = -0;
		if (key == GLFW_MOUSE_BUTTON_LEFT)
		{
			// Unhides cursor since camera is not looking around anymore
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			// Makes sure the next time the camera looks around it doesn't jump
			firstClick = true;
		}

	}

}


//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
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

	ShaderProgram* sp = new ShaderProgram("shader_v.glsl", NULL, "shader_f.glsl");

	// Initialize the scene and set shader program
	scene_mgr.init();
	GLuint tex = scene_mgr.readTexture("clownfish.png");
	scene_mgr.setShaderProgram(sp);

	auto clownfish_model = Objects::Model("clownfish.obj");
	auto fish = new Objects::Fish(tex, clownfish_model);
	scene_mgr.addObject(fish);

	float angle_y = 0; //zadeklaruj zmienn¹ przechowuj¹c¹ aktualny k¹t obrotu
	float angle_x = 0;
	glfwSetTime(0);
	double lastTime = 0;

	//Main application loop
	while (!glfwWindowShouldClose(scene_mgr.window)) //As long as the window shouldnt be closed yet...
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		angle_y += speed_y * deltaTime;
		angle_x += speed_x * deltaTime;

		mat4 Mc = rotate(mat4(1.0f), angle_y, vec3(0, 1, 0));
		Mc = rotate(Mc, angle_x, vec3(1, 0, 0));
		vec4 dir_ = Mc * vec4(0, 0, 1, 0);
		dir = vec3(dir_);

		vec3 mdir = normalize(vec3(dir.x, 0, dir.z));

		pos += ws * (float)deltaTime * mdir;

		scene_mgr.draw();
		scene_mgr.performMoves(currentTime, currentTime - lastTime);
		lastTime = currentTime;

		glfwPollEvents();
	}

	scene_mgr.destroy();
	delete sp;

	glDeleteTextures(1, &tex);
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}
