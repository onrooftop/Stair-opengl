#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 1.0f);

	FragPos = vec3(model * vec4(aPos, 1.0f));

	Normal = mat3(transpose(inverse(model))) * aNormal;

	TexCoord = aTexCoords;
}