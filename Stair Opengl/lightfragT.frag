#version 440 core


struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutOff;
	float cutOffOutter;
	vec3 direction;
};


in vec2 TexCoords;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;


uniform vec3 viewPos;
uniform Material material;
uniform Light light;


void main()
{

	vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(lightDir, norm),0.0);
	vec3 diffuse = (diff * vec3(texture(material.diffuse, TexCoords))) * light.diffuse;

	

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (spec * vec3(texture(material.specular, TexCoords))) * light.specular;




	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.cutOffOutter;
	float intensity = clamp((theta - light.cutOffOutter) / epsilon, 0.0, 1.0);
	
	diffuse *= intensity;
	specular *= intensity;

	vec3 result = (diffuse + ambient + specular);
	
	FragColor = vec4(result, 1.0);

}