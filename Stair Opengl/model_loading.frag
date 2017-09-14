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


out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 viewPos;

uniform PointLight pointLights[1];

vec3 calcPointLight(PointLight light, vec3 normal,vec3 fragPos, vec3 viewDir);

void main()
{    
	vec3 norm = normalize(Normal);
	vec3 result = vec3(0.0);
	vec3 viewDir = normalize(viewPos - FragPos);

	for(int i = 0; i < 1; i++)
	{
		result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
	}

    FragColor = vec4(result, 1.0f);
}

vec3 calcPointLight(PointLight light, vec3 normal,vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoord));
	
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * vec3(texture(texture_diffuse1, TexCoord)) * diff;
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow( max( dot(viewDir, reflectDir) ,0.0 ), 32.0f);
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoord));

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);

}