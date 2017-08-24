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




Camera cam(glm::vec3(0.0f, feet + 0.3f, 1.0f));


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

	glm::vec3 color[30];
	for (int i = 0; i < 30; i++)
	{
		color[i] = glm::vec3(1.0f / i, 0.0f / i, 1.0f / i);
	}
	
	float xMin = -0.5f;
	float xMax = 0.5f;
	float yMin = -0.5f;
	float yMax = 0.5f;
	float zMin = 0.5f;
	float zMax = -0.5f;

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



		float w = 0.5f;
		float h = 0.075f;
		glm::vec4 testMin, testMax, temp;
		for (int i = 0; i < 30; i++)
		{
			model = glm::mat4();
		//	model = glm::rotate(model, glm::radians(45.0f * 1), glm::vec3(0.0f, 1.0f, 0.0f));

			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.2f * i));

			model = glm::scale(model, glm::vec3(w, h*(i+1), 0.2f));

			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));

			myShader.setVec3("col", color[i]);
			myShader.setMat4("model", model);
		

			

			temp = model * vertices[0];
			testMin = glm::vec4(temp);
			testMax = glm::vec4(temp);


			for (int j = 1; j < 8; j++)
			{
				temp = model * vertices[j];

				if (testMin.x >= temp.x)
					testMin.x = temp.x;

				if (testMax.x < temp.x)
					testMax.x = temp.x;

				if (testMin.y >= temp.y)
					testMin.y = temp.y;

				if (testMax.y < temp.y)
					testMax.y = temp.y;

				if (testMin.z >= temp.z)
					testMin.z = temp.z;

				if (testMax.z < temp.z)
					testMax.z = temp.z;

			}

			//std::cout << testMax.z << testMin.z << '\n';
			std::cout << feet << '\n';
			

			if ((cam.pos.x >= testMin.x && cam.pos.x <= testMax.x) &&
				(cam.pos.z >= testMin.z && cam.pos.z <= testMax.z) &&
				(feet + h + h/2.0f>= testMax.y))
			{
				feet = testMax.y;
				cam.pos.y = 0.3f + feet;
				onStep = true;
				std::cout << i << '\n';


			}
			if (i == 5)
			{
				std::cout << testMax.y << std::endl;
			}


			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			//std::cout << "1";
		}
		
		if (!onStep)
		{
			feet = 0.0f;
			cam.pos.y = 0.3f + feet;
			//std::cout << '\n';
		}


		view = cam.getView();
		myShader.setMat4("view", view);
		
		glDisable(GL_DEPTH_TEST);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(cam.pos.x, 0.0f, cam.pos.z));
		model = glm::translate(model, glm::vec3(0.5f, -0.5f, -0.5f));
		myShader.setMat4("model", model);
		myShader.setVec3("col", glm::vec3(0.0f));
		glPointSize(3.0f);
		glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);

	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);

	

		glEnable(GL_DEPTH_TEST);

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

