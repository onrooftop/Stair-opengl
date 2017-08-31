#version 440 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
	FragColor = texture(ourTexture, TexCoord);
}