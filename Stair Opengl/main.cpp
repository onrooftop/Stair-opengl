#include <iostream>

#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>

#include <glad\glad.h>
#include <GLFW\glfw3.h>

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

	unsigned int VAO, VBO;

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_data), vertices_data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int diffuseMap = load_texture("d:\\Opengl\\mypic\\container2.png");
	unsigned int specularMap = load_texture("d:\\Opengl\\mypic\\container2_specular.png");

	unsigned int tile = load_texture("d:\\Opengl\\mypic\\tile.jpg");

	light.use();
	light.setInt("material.diffuse", 0);
	light.setInt("material.specular", 1);

	glm::mat4 proj;
	glm::mat4 view;
	glm::mat4 model;

	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


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

		glBindVertexArray(VAO);
		
		proj = glm::perspective(glm::radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
		view = cam.getView();


		lamp.use();
		proj = glm::perspective(glm::radians(cam.fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
		view = cam.getView();



		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));

		lamp.setMat4("proj", proj);
		lamp.setMat4("view", view);
		lamp.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 8.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		lamp.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		light.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);


		light.setVec3("viewPos", cam.pos);
		light.setFloat("material.shininess", 32.0f);

		light.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		light.setVec3("dirLight.ambient", glm::vec3(0.05f));
		light.setVec3("dirLight.diffuse", glm::vec3(0.4f));
		light.setVec3("dirLight.specular", glm::vec3(0.0f));

		light.setVec3("pointLights[0].position", glm::vec3(0.0f, 2.0f, 0.0f));
		light.setVec3("pointLights[0].ambient",  glm::vec3(0.05f, 0.05f, 0.05f));
		light.setVec3("pointLights[0].diffuse",  glm::vec3(0.8f, 0.8f, 0.8f));
		light.setVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		light.setFloat("pointLights[0].constant",1.0f);
		light.setFloat("pointLights[0].linear",  0.09);
		light.setFloat("pointLights[0].quadratic",0.032);

		light.setVec3("pointLights[1].position", glm::vec3(0.0f, 8.0f, 0.0f));
		light.setVec3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		light.setVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		light.setVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		light.setFloat("pointLights[1].constant", 1.0f);
		light.setFloat("pointLights[1].linear", 0.09);
		light.setFloat("pointLights[1].quadratic", 0.032);

		if (toggleFlashlight)
		{
			light.setVec3("spotlight.position", cam.pos);
			light.setVec3("spotlight.direction", cam.front);
			light.setVec3("spotlight.ambient", glm::vec3(0.0f));
			light.setVec3("spotlight.diffuse", glm::vec3(1.0f));
			light.setVec3("spotlight.specular", glm::vec3(1.0f));
			light.setFloat("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
			light.setFloat("spotlight.cutOffOutter", glm::cos(glm::radians(17.5f)));
		}
		else
		{
			light.setVec3("spotlight.position", cam.pos);
			light.setVec3("spotlight.direction", cam.front);
			light.setVec3("spotlight.ambient", glm::vec3(0.0f));
			light.setVec3("spotlight.diffuse", glm::vec3(0.0f));
			light.setVec3("spotlight.specular", glm::vec3(0.0f));
			light.setFloat("spotlight.cutOff", glm::cos(glm::radians(0.0f)));
			light.setFloat("spotlight.cutOffOutter", glm::cos(glm::radians(0.0f)));
		}





		float w = 0.5f;
		float h = 0.075f;
		float feetMin = 0;

		glm::vec4 testMin, testMax, temp;


		for (int i = 0; i < 100; i++)
		{

			light.setMat4("proj", proj);
			light.setMat4("view", view);

			model = glm::mat4();
			model = glm::rotate(model, glm::radians(10.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));

			model = glm::translate(model, glm::vec3(0.0f, h * i, 1.0f));

			model = glm::scale(model, glm::vec3(0.3f, h, w));

			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));

			light.setMat4("model", model);
		

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
				feetMin = min(p0.y, feet);
				feet = max(p0.y, feet);
				cam.pos.y = 0.3f + feet;
				onStep = true;
				//std::cout << i << '\n';

			}
			
			
		

			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
		
		if (!onStep)
		{
			feet = 0.0f;
			cam.pos.y = 0.3f + feet;
		}
		else if (feetMin < feet)
		{
			feet = feetMin;
			cam.pos.y = 0.3f + feet;
		}

		light.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tile);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tile);
		
		model = glm::mat4();

		model = glm::translate(model, glm::vec3(0.0f, -0.05f, 0.0f));

		model = glm::scale(model, glm::vec3(400.0f, 0.1f, 400.0f));


		light.setMat4("proj", proj);
		light.setMat4("view", view);
		light.setVec3("col", glm::vec3(0.0f, 1.0f, 0.0f));
		light.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	glDeleteVertexArrays(1, &VAO);
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
