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

Camera::Camera(glm::vec3 _pos, glm::vec3 _up, float _yaw, float _pitch, bool _fly) 
	: front(glm::vec3(0.0f, 0.0f, -1.0f))
	, move_speed(MOVE_SPEED), mouse_sensitivity(MOUSE_SENSITIVITY)
	, world_up(glm::vec3(0.0f, 1.0f, 0.0f))
	, fov(FOV)
{
	pos = _pos;
	up = _up;
	yaw = _yaw;
	pitch = _pitch;
	fly = _fly;
	update();
}

void Camera::keyboard_move(Camera_Movement move, float deltaTime)
{
	float y;
	//if (!fly)
	//{
	//	y = pos.y;
	//}
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

	//if (!fly)
	//{
	//	pos.y = y;
	//}
	
}

void Camera::mouse_move(float xoffset, float yoffset)
{
	xoffset *= mouse_sensitivity;
	yoffset *= mouse_sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	update();
}

void Camera::mouse_scroll(float yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

glm::mat4 Camera::getView()
{
	return glm::lookAt(pos, pos + front, up);
}
