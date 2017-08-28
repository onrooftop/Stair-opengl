#include <iostream>

#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "Shader.h"
#include "Camera.h"


void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int w, int h);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);



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




Camera cam(glm::vec3(0.0f, feet + 0.3f, 2.0f));


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



	glm::vec4 vertices[] = {
		glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f),
		glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f),
		glm::vec4( 0.5f, -0.5f,  0.5f, 1.0f),
		glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f),
		glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f),
		glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),
		glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f),
		glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f)
	};

	float vertices2[] = {
		-0.5f,  0.5f,  0.5f,  //v1
		-0.5f, -0.5f,  0.5f,  //v2
		 0.5f, -0.5f,  0.5f,  //v3
		 0.5f,  0.5f,  0.5f,  //v4
		-0.5f,  0.5f, -0.5f,  //v5
		-0.5f, -0.5f, -0.5f,  //v6
		 0.5f, -0.5f, -0.5f,  //v7
		 0.5f,  0.5f, -0.5f   //v8
	
	};

	unsigned int indices[] = {
		0,1,2,
		0,2,3,
		4,5,6,
		4,6,7,
		4,5,1,
		4,1,0,
		3,2,6,
		3,6,7,
		4,0,3,
		4,3,7,
		1,5,6,
		1,6,2
	};

	Shader myShader("vshader.vert", "fshader.frag");

	unsigned int VAO, VBO, EBO;

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glm::mat4 proj;
	glm::mat4 view;
	glm::mat4 model;



	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::vec3 color[2];

	color[0] = glm::vec3(1.0f, 0.0f, 1.0f);
	color[1] = glm::vec3(0.0f, 1.0f, 1.0f);

	

	bool onStep;

	while (!glfwWindowShouldClose(window))
	{
		onStep = false;
		canmove = true;

		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myShader.use();
		glBindVertexArray(VAO);

		proj = glm::perspective(glm::radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
	
		

		myShader.setMat4("proj", proj);
		view = cam.getView();
		myShader.setMat4("view", view);


		float w = 0.5f;
		float h = 0.075f;

		glm::vec4 testMin, testMax, temp;


		for (int i = 0; i < 100; i++)
		{
			model = glm::mat4();
			model = glm::rotate(model, glm::radians(10.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));

			model = glm::translate(model, glm::vec3(0.0f, h * i, 1.0f));

			model = glm::scale(model, glm::vec3(w, h, 0.3f));

			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));

			myShader.setVec3("col", color[i % 2]);

			myShader.setMat4("model", model);
		

			glm::vec4 temp2[8];

			for (int j = 0; j < 8; j++)
			{
				temp2[j] = model * vertices[j];
			}

			glm::vec3 p0 = glm::vec3(temp2[0].x, temp2[0].y, temp2[0].z);
			glm::vec3 p3 = glm::vec3(temp2[3].x, temp2[3].y, temp2[3].z);
			glm::vec3 p4 = glm::vec3(temp2[4].x, temp2[4].y, temp2[4].z);
			glm::vec3 p7 = glm::vec3(temp2[7].x, temp2[7].y, temp2[7].z);


			glm::vec3 vec0 = (p3 - p0);
			glm::vec3 vec3 = (p7 - p3);
			glm::vec3 vec4 = (p0 - p4);
			glm::vec3 vec7 = (p4 - p7);


			glm::vec3 rightV0 = glm::normalize(glm::cross(vec0, glm::vec3(0.0f, 1.0f, 0.0f)));
			glm::vec3 rightV3 = glm::normalize(glm::cross(vec3, glm::vec3(0.0f, 1.0f, 0.0f)));
			glm::vec3 rightV4 = glm::normalize(glm::cross(vec4, glm::vec3(0.0f, 1.0f, 0.0f)));
			glm::vec3 rightV7 = glm::normalize(glm::cross(vec7, glm::vec3(0.0f, 1.0f, 0.0f)));


			glm::vec3 projed0 = glm::dot(glm::normalize(vec0), cam.pos - p0) / glm::length(vec0) * (vec0) + p0;
			glm::vec3 projed3 = glm::dot(glm::normalize(vec3), cam.pos - p3) / glm::length(vec3) * (vec3) + p3;
			glm::vec3 projed4 = glm::dot(glm::normalize(vec4), cam.pos - p4) / glm::length(vec4) * (vec4) + p4;
			glm::vec3 projed7 = glm::dot(glm::normalize(vec7), cam.pos - p7) / glm::length(vec7) * (vec7) + p7;


			glm::vec3 vecp0 = (cam.pos - projed0);
			glm::vec3 vecp3 = (cam.pos - projed3);
			glm::vec3 vecp4 = (cam.pos - projed4);
			glm::vec3 vecp7 = (cam.pos - projed7);


			float dotVecP0 = glm::dot(rightV0, vecp0);
			float dotVecP3 = glm::dot(rightV3, vecp3);
			float dotVecP4 = glm::dot(rightV4, vecp4);
			float dotVecP7 = glm::dot(rightV7, vecp7);


			if ((dotVecP0 <= 0 && dotVecP3 <= 0) &&
				(dotVecP4 <= 0 && dotVecP7 <= 0) &&
				(feet + h + h / 2.0f >= p0.y))
			{
				feet = p0.y;
				cam.pos.y = 0.3f + feet;
				onStep = true;
				//std::cout << i << '\n';

			}


			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		}
		
		if (!onStep)
		{
			feet = 0.0f;
			cam.pos.y = 0.3f + feet;
		}

		
		model = glm::mat4();

		model = glm::translate(model, glm::vec3(0.0f, -0.05f, 0.0f));

		model = glm::scale(model, glm::vec3(400.0f, 0.1f, 400.0f));

		myShader.setVec3("col", glm::vec3(0.0f, 1.0f, 0.0f));
		myShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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

