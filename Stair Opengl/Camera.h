
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
const float MOUSE_SENSITIVITY = 0.1f;


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


	Camera(glm::vec3 _pos, glm::vec3 _up, float _yaw, float _pitch);
	void move(Camera_Movement move, bool fly, float deltaTime);

};

#endif // !CAMERA_H