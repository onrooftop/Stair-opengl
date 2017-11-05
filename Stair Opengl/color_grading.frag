#version 440 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D lut;

void main()
{
	vec3 col = texture2D(screenTexture, TexCoords).rgb;


	vec2 colInLut;

	colInLut.x = (col.r * 15 + col.b * 240) / 256;;
	colInLut.y = (1 - col.g);




	vec3 color = texture2D(lut, colInLut).rgb;

	FragColor = vec4(color.rgb, 1.0);
} 