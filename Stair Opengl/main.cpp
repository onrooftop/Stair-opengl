#include <iostream>

#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int w, int h);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
unsigned int load_texture(const char *path);

const int WIDTH = 800;
const int HEIGHT = 600;

const char *WINDOW_NAME = "Stair";

float deltaTime = 0.0;
float lastTime = 0.0;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
float feet = 0.0f;

bool canmove = true;
bool firstmouse = true;
bool toggleFlashlight = true;

Camera cam(glm::vec3(0.0f, 0.0f, 5.0f));

float rolls[3] = {0.0f, 0.0f, 0.0f};
float pitches[3] = { 0.0f, 0.0f, 0.0f };


unsigned int mode = 0;



int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_NAME, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "FAILED TO CREATE WINDOW\n";
		glfwTerminate();
		return -1;
	}

	glfwSetWindowPos(window, 400, 400);
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "FAILED TO LOAD OPENGL\n";
		return -1;
	}




	glm::mat4 proj;
	glm::mat4 view;

	Shader shader("v.vert", "f.frag");
	Shader lamb("vshaderNoLight.vert", "fshaderNoLight.frag");

	
	//Model ourModel("D:\\Opengl\\Stair Opengl\\Model\\nanosuit\\nanosuit.obj");
	Model *ourModel = new Model("D:\\Opengl\\Stair Opengl\\Model\\Humanoid_robot.fbx");

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);




	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{

		canmove = true;

		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		processInput(window);

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		proj = glm::perspective(glm::radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
		view = cam.getView();





		shader.use();

		shader.setVec3("pointLights[0].position", glm::vec3(0.0f, 5.0f, 3.0f));
		shader.setVec3("viewPos", cam.pos);

		// light properties
		shader.setVec3("pointLights[0].ambient", glm::vec3(0.2f));
		shader.setVec3("pointLights[0].diffuse", glm::vec3(0.5f));
		shader.setVec3("pointLights[0].specular", glm::vec3(1.0f));
		shader.setFloat("pointLights[0].constant", 1.0f);
		shader.setFloat("pointLights[0].linear", 0.09f);
		shader.setFloat("pointLights[0].quadratic", 0.032f);

		shader.setMat4("proj", proj);
		shader.setMat4("view", view);

		glm::mat4 model;
		//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		//shader.setMat4("model", model);

		//ourModel->RotArm(mode, roll);

		ourModel->Draw(shader, model, mode, rolls[mode], pitches[mode]);



		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();

	return 0;
}


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.keyboard_move(FORWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.keyboard_move(BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.keyboard_move(LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.keyboard_move(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		toggleFlashlight = true;

	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
	{
		toggleFlashlight = false;
	}
		

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{

		mode = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{

		mode = 1;

	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{

		mode = 2;

	}



	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		rolls[mode] += 10*deltaTime;
		std::cout << rolls[mode] << "\n";
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		rolls[mode] -= 0.7 *deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		pitches[mode] += 0.7*deltaTime;
	}


	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		pitches[mode] -= 0.7*deltaTime;
	}
	

}


void framebuffer_size_callback(GLFWwindow *window, int w, int h)
{
	glViewport(0, 0, w, h);
}


void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (firstmouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstmouse = false;
	}


	float xoffset, yoffset;

	xoffset = xpos - lastX;
	yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	cam.mouse_move(xoffset, yoffset);


}


void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	cam.mouse_scroll(yoffset);
}

