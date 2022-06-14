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
#include "constants.h"
#include "fish.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "scene_mgr.h"
#include "file_model.h"
#include "ground_model.h"
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
		if (key == GLFW_KEY_UP) ws = 7;
		if (key == GLFW_KEY_DOWN) ws = -7;
		
		if (key == GLFW_MOUSE_BUTTON_LEFT){
			// Hides mouse cursor
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			// Prevents camera from jumping on the first click
			if (firstClick){
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
			if (abs(glm::angle(newOrientation,Up) - glm::radians(90.0f)) <= glm::radians(85.0f)){
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
		if (key == GLFW_MOUSE_BUTTON_LEFT){
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
	GLuint tex_sand = scene_mgr.readTexture("textures/Sand_COLOR.png");
	GLuint tex_clownfish = scene_mgr.readTexture("textures/clownfish.png");
	GLuint tex_barbfish = scene_mgr.readTexture("textures/Tiger_barb_diff.png");
	GLuint tex_ye_fish = scene_mgr.readTexture("textures/12988_Electric_Yellow_Cichlid_diff.png");
	GLuint tex_coral_1 = scene_mgr.readTexture("textures/model.png");
	GLuint tex_coral_2 = scene_mgr.readTexture("textures/12.png");
	GLuint tex_coral_3 = scene_mgr.readTexture("textures/Coral-Academy_material_1.png");
	GLuint tex_coral_4 = scene_mgr.readTexture("textures/RowanMcLachlan_Coral-1.png");

	scene_mgr.setShaderProgram(sp);
	auto ground_model = Objects::GroundModel(56);
	auto ground = new Objects::Object(tex_sand, &ground_model);
	ground->setPosition(0, -5, 0);
	scene_mgr.addObject(ground);

	auto clownfish_model = Objects::FileModel("models/clownfish.obj");
	for (int i = 0; i < 10; i++) {
		auto fish = new Objects::Fish(tex_clownfish, &clownfish_model);
		fish->setPosition(rand() % 80 - 40, rand() % 20 - 5, rand() % 80 - 40);
		fish->setRotation((float)fmod(rand(), 2*PI), 0);
		float scale = 0.5 + (rand() % 100) / 100.0;
		fish->setScale(scale);
		scene_mgr.addObject(fish);
	}

	auto barb_model = Objects::FileModel("models/barb.obj");
	for (int i = 0; i < 10; i++) {
		auto barb = new Objects::Fish(tex_barbfish, &barb_model);
		barb->setPosition(rand() % 60 - 30, rand() % 20 - 5, rand() % 60 - 30);
		barb->setRotation((float)fmod(rand(), 2 * PI), 0);
		float scale = 0.5 + (rand() % 100) / 100.0;
		barb->setScale(scale);
		scene_mgr.addObject(barb);
	}

	auto ye_model = Objects::FileModel("models/yellowelectric.obj");
	for (int i = 0; i < 10; i++) {
		auto ye = new Objects::Fish(tex_ye_fish, &ye_model);
		ye->setPosition(rand() % 60 - 30, rand() % 20 - 5, rand() % 60 - 30);
		ye->setRotation(fmod((float)rand(), 2 * PI), 0);
		float scale = 0.5 + (rand() % 100) / 100.0;
		ye->setScale(scale);
		scene_mgr.addObject(ye);
	}

	auto Coral_model_2 = Objects::FileModel("models/coral2.obj");
	for (int i = 0; i < 20; i++) {
		auto Coral_2 = new Objects::Object(tex_coral_2, &Coral_model_2);
		Coral_2->setPosition(rand() % 80, -5.1, rand() % 80);
		float scale = 0.5 + (rand() % 100) / 100.0;
		Coral_2->setScale(scale);
		scene_mgr.addObject(Coral_2);
	}

	auto Coral_model_3 = Objects::FileModel("models/coral3.obj");
	for (int i = 0; i < 20; i++) {
		auto Coral_3 = new Objects::Object(tex_coral_3, &Coral_model_3);
		float scale = 0.5 + (rand() % 100) / 100.0;
		Coral_3->setPosition(rand() % 80,-7 + scale, rand() % 80);
		Coral_3->setScale(scale);
		scene_mgr.addObject(Coral_3);
	}

	auto Coral_model_4 = Objects::FileModel("models/coral4.obj");
	for (int i = 0; i < 10; i++) {
		auto Coral_4 = new Objects::Object(tex_coral_4, &Coral_model_4);
		float scale = 0.3 + (rand() % 100) / 200.0;
		Coral_4->setPosition(rand() % 100, rand() % 5 - 10 + 7 * scale, rand() % 200);
		Coral_4->setScale(scale);
		scene_mgr.addObject(Coral_4);
	}


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
		angle_x = glm::clamp(angle_x + speed_x * (float)deltaTime, -PI/2 + 0.001f, PI/2 - 0.001f);

		mat4 Mc = rotate(mat4(1.0f), angle_y, vec3(0, 1, 0));
		Mc = rotate(Mc, angle_x, vec3(1, 0, 0));
		vec4 dir_ = Mc * vec4(0, 0, 1, 0);
		dir = vec3(dir_);
		
		vec3 mdir = normalize(vec3(dir.x, dir.y, dir.z));

		pos += ws * (float)deltaTime * mdir;
		
		ground->setPosition(pos.x - fmod(pos.x, 4), ground->y, pos.z - fmod(pos.z, 4));
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Coral->setPosition(pos.x - fmod(pos.x, 30), Coral->y, pos.z - fmod(pos.z, 30));

		scene_mgr.draw();
		scene_mgr.performMoves(currentTime, currentTime - lastTime);
		lastTime = currentTime;

		glfwPollEvents();
	}

	scene_mgr.destroy();
	delete sp;

	glDeleteTextures(1, &tex_clownfish);
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//glDeleteTextures(1, &tex_coral_1);
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}
