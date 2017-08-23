#include "Camera.h"

void Camera::update()
{
	glm::vec3 _front;
	
	_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	_front.y = sin(glm::radians(pitch));
	_front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	front = glm::normalize(_front);

	right = glm::normalize(glm::cross(front, world_up));
	up = glm::normalize(glm::cross(right, front));
}

Camera::Camera(glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f), float _yaw = YAW, float _pitch = PITCH):
	front(glm::vec3(0.0f, 0.0f, -1.0f)), move_speed(MOVE_SPEED), mouse_sensitivity(MOUSE_SENSITIVITY), world_up(glm::vec3(0.0f, 1.0f, 0.0f))
{
	pos = _pos;
	up = _up;
	yaw = _yaw;
	pitch = _pitch;
	update();
}

void Camera::move(Camera_Movement move, bool fly = false,float deltaTime)
{
	float y;
	if (!fly)
	{
		y = pos.y;
	}
	if (move == FORWARD)
	{
		pos += front * move_speed * deltaTime;
	}

	if (move == BACKWARD)
	{
		pos -= front * move_speed * deltaTime;
	}

	if (move == RIGHT)
	{
		pos += right * move_speed * deltaTime;
	}

	if (move == LEFT)
	{
		pos -= right * move_speed * deltaTime;
	}

	if (!fly)
	{
		pos.y = y;
	}
	
}
