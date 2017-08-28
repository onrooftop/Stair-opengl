#version 440 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;


uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;




uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec3 result;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 1.0f);
	vec3 FragPos = vec3(model * vec4(aPos, 1.0f));
	vec3 Normal = mat3(transpose(inverse(model))) * aNormal;  




	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(lightDir, norm),0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = spec * lightColor * specularStrength;

	result = (diffuse + ambient + specular);
}