#version 440 core


struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 direction;
	float cutOff;
};




in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;


uniform vec3 viewPos;
uniform Material material;
uniform Light light;


void main()
{

	vec3 ambient = material.ambient * light.ambient;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(lightDir, norm),0.0);
	vec3 diffuse = (diff * material.diffuse) * light.diffuse;


	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256.0);
	vec3 specular = (spec * material.specular) * light.specular;

	vec3 result = (diffuse + ambient + specular);
	FragColor = vec4(result, 1.0f);
}