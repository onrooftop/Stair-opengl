
#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>


#include <GLFW\glfw3.h>


enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


const float YAW = -90.0f;
const float PITCH = 0.0f;
const float MOVE_SPEED = 0.75f;
const float MOUSE_SENSITIVITY = 0.2f;
const float FOV = 45.0f;


class Camera
{
private:
	void update();
public:
	glm::vec3 pos;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 front;
	glm::vec3 world_up;

	float move_speed;
	float mouse_sensitivity;
	float yaw;
	float pitch;
	float fov;

	bool fly;


	Camera(glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f), float _yaw = YAW, float _pitch = PITCH, bool fly = false);
	void keyboard_move(Camera_Movement move, float deltaTime);
	void mouse_move(float xoffset, float yoffset);
	void mouse_scroll(float yoffset);
	glm::mat4 getView();

};

#endif // !CAMERA_H