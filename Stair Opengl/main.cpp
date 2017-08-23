#include <iostream>

#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "Shader.h"


void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int w, int h);

const int WIDTH = 800;
const int HEIGHT = 600;

const char *WINDOW_NAME = "Stair";



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

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "FAILED TO LOAD OPENGL\n";
		return -1;
	}



	float vertices[] = {
		-0.5f, -0.5f, -0.5f, // 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  //1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  //1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  //1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, // 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, // 0.0f, 0.0f,
							 //
		-0.5f, -0.5f,  0.5f, // 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  //1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  //1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  //1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, // 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, // 0.0f, 0.0f,
							 //
		-0.5f,  0.5f,  0.5f, // 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, // 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, // 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, // 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, // 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, // 1.0f, 0.0f,
							 //
		0.5f,  0.5f,  0.5f,  //1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  //1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  //0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  //0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  //0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  //1.0f, 0.0f,
							 //
		-0.5f, -0.5f, -0.5f, // 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  //1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  //1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  //1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, // 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, // 0.0f, 1.0f,
							 //
		-0.5f,  0.5f, -0.5f, // 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  //1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  //1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  //1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, // 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f // 0.0f, 1.0f
	};

	Shader myShader("vshader.vert", "fshader.frag");

	unsigned int VAO, VBO;

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myShader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


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
}

void framebuffer_size_callback(GLFWwindow *window, int w, int h)
{
	glViewport(0, 0, w, h);
}