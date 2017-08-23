#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <string>
#include <fstream>
#include <sstream>

class Shader
{

private:
	void checkCompileErrors(unsigned int shader, std::string type);
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();



};

#endif // !SHADER_H




