#ifndef MESH_H

#include <glm\glm\glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 TexCoord;
};

struct Texture
{
	unsigned int ID;
	std::string type;
};



class Mesh
{
private:
	unsigned int VAO, VBO, EBO;

	void setupMesh();
public:

	~Mesh();

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void Draw(Shader shader);
};


#endif // !MESH_H
