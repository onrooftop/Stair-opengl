#version 440 core

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Spotlight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float cutOffOutter;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};

struct Material
{
	sampler2D diffuse;
	sampler2D specular;

	float shininess;
};


out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;


#define NR_POINT_LIGHT 2
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHT];
uniform Spotlight spotlight;

uniform Material material;
uniform vec3 viewPos;


vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(Spotlight spotlight, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 result = vec3(0.0);
	vec3 viewDir = normalize(viewPos - FragPos);


	result += calcDirLight(dirLight, norm, viewDir);

	for(int i = 0; i < NR_POINT_LIGHT; i++)
	{
		result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
	}

	//result += calcSpotLight(spotlight, norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoord)) * diff;
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow( max( dot(viewDir, reflectDir) ,0.0 ), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
	
	return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal,vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoord)) * diff;
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow( max( dot(viewDir, reflectDir) ,0.0 ), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);

}

vec3 calcSpotLight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{

	vec3 lightDir = normalize(light.position - fragPos);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoord)) * diff;
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow( max( dot(viewDir, reflectDir) ,0.0 ), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.cutOffOutter;
	float intensity = clamp((theta - light.cutOffOutter) / epsilon, 0.0, 1.0);

	ambient *= intensity;
	diffuse *= intensity;
	specular *= intensity;
	
	return (ambient + diffuse + specular);

}


