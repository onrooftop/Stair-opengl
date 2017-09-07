#include <iostream>

#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <stdlib.h>
#include <time.h>

#include "Shader.h"
#include "Camera.h"

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

Camera cam(glm::vec3(0.0f, 3.0f, 2.0f));


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

	float vertices_data[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	Shader light("vshader.vert", "fshader.frag");
	Shader lamp("vshaderNoLight.vert", "fshaderNoLight.frag");

	unsigned int pVAO, lVAO, VBO;

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &pVAO);

	glBindVertexArray(pVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_data), vertices_data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glGenVertexArrays(1, &lVAO);
	glBindVertexArray(lVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);




	glm::mat4 proj;
	glm::mat4 view;
	glm::mat4 model;

	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	srand(time(0));
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	float lightColorX;
	float lightColorY;
	float lightColorZ;

	glm::vec3 cols[5];


	for (int i = 0; i < 5; i++)
	{
		lightColorX = (rand() % 255) / 255.0f;
		lightColorY = (rand() % 255) / 255.0f;
		lightColorZ = (rand() % 255) / 255.0f;
		cols[i] = glm::vec3(lightColorX, lightColorY, lightColorZ);
	}


	while (!glfwWindowShouldClose(window))
	{

		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		processInput(window);

		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		proj = glm::perspective(glm::radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
		view = cam.getView();

		glBindVertexArray(pVAO);





		light.use();
		light.setVec3("dirLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
		light.setVec3("dirLight.lightCol", glm::vec3(0.1f));





		//point light  1

		for (int i = 0; i < 20; i++)
		{

			float y = sin(currentTime*2.5 + i)+1;


			if (y - 0.001 <= 0.0f && i == 0)
			{
				cols[0].x = (rand() % 255) / 255.0f;
				cols[0].y = (rand() % 255) / 255.0f;
				cols[0].z = (rand() % 255) / 255.0f;

			}


			glm::vec3 color = glm::vec3(lightColorX, lightColorY, lightColorZ);

			model = glm::mat4();
			model = glm::rotate(model, glm::radians(360/ 20.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(1.5f, y/2.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

			glm::vec3 r = model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			//std::cout << r.x << " " << r.y << " " << r.z << "\n";


			light.use();

			std::string point;
			point = "pointLights[" + std::to_string(i) + "].";

			light.setVec3(point + "position", r);
			light.setVec3(point + "lightCol", cols[0]);
			light.setFloat(point + "constant", 1.0f);
			light.setFloat(point + "linear", 0.7f);
			light.setFloat(point + "quadratic", 1.80f);

	

			lamp.use();

			lamp.setVec3("col", cols[0]);
			lamp.setMat4("proj", proj);
			lamp.setMat4("view", view);
			lamp.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		//point light  1





		////point light  2
		for (int i = 20; i < 40; i++)
		{

			float y = cos(currentTime*4 + i) + 1;

			//std::cout << y << "\n";
			if (y - 0.001 <= 0.0f && i == 20)
			{
				cols[1].x = (rand() % 255) / 255.0f;
				cols[1].y = (rand() % 255) / 255.0f;
				cols[1].z = (rand() % 255) / 255.0f;

				
			}


			glm::vec3 color = glm::vec3(lightColorX, lightColorY, lightColorZ);

			model = glm::mat4();
			model = glm::rotate(model, glm::radians(360 / 20.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(3.0f, y/2.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

			glm::vec3 r = model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			//std::cout << r.x << " " << r.y << " " << r.z << "\n";


			light.use();

			std::string point;
			point = "pointLights[" + std::to_string(i) + "].";

			light.setVec3(point + "position", r);
			light.setVec3(point + "lightCol", cols[1]);
			light.setFloat(point + "constant", 1.0f);
			light.setFloat(point + "linear", 0.7f);
			light.setFloat(point + "quadratic", 1.80f);



			lamp.use();

			lamp.setVec3("col", cols[1]);
			lamp.setMat4("proj", proj);
			lamp.setMat4("view", view);
			lamp.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		////point light  2


		////point light  3
		for (int i = 40; i < 50; i++)
		{

			float y = cos(currentTime * 5 + i) + 1;

			//std::cout << y << "\n";
			if (y - 0.001 <= 0.0f && i == 40)
			{
				cols[2].x = (rand() % 255) / 255.0f;
				cols[2].y = (rand() % 255) / 255.0f;
				cols[2].z = (rand() % 255) / 255.0f;


			}


			glm::vec3 color = glm::vec3(lightColorX, lightColorY, lightColorZ);

			model = glm::mat4();
			model = glm::rotate(model, glm::radians(360 / 10.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(1.0f, y, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

			glm::vec3 r = model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			//std::cout << r.x << " " << r.y << " " << r.z << "\n";


			light.use();

			std::string point;
			point = "pointLights[" + std::to_string(i) + "].";

			light.setVec3(point + "position", r);
			light.setVec3(point + "lightCol", cols[2]);
			light.setFloat(point + "constant", 1.0f);
			light.setFloat(point + "linear", 0.7f);
			light.setFloat(point + "quadratic", 1.80f);



			lamp.use();

			lamp.setVec3("col", cols[2]);
			lamp.setMat4("proj", proj);
			lamp.setMat4("view", view);
			lamp.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		////point light  3


		//plane
		light.use();
		model = glm::mat4();
		model = glm::scale(model, glm::vec3(300.0f, 0.1f, 300.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));


		light.setMat4("proj", proj);
		light.setMat4("view", view);
		light.setMat4("model", model);
		light.setVec3("viewPos", cam.pos);

		light.setVec3("material.matColor", glm::vec3(0.1f));
		light.setFloat("material.shininess", 32.0f);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//plane


		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	glDeleteVertexArrays(1, &pVAO);
	glDeleteVertexArrays(1, &lVAO);
	glDeleteBuffers(1, &VBO);
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


unsigned int load_texture(const char *path)
{
	unsigned int textureID;

	glGenTextures(1, &textureID);
	

	int width, height, nChannel;
	unsigned char* data = stbi_load(path, &width, &height, &nChannel, 0);
	
	if (data)
	{
		GLenum format;
		if (nChannel == 3)
			format = GL_RGB;
		else if (nChannel == 4)
			format = GL_RGBA;


		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	}
	else
	{
		std::cout << "Texture failed to load image\n";
	}


	stbi_image_free(data);
	return textureID;
}
