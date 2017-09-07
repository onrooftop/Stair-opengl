#pragma once

#include <glm\glm\glm.hpp>

class Pointlight
{
public:
	Pointlight();
	~Pointlight();

	void update();

private:
	glm::vec3 _lightColor;
	glm::vec3 _position;
	
	unsigned int _id;

};

